#include "Database.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

// Remove trailing carriage return / newline characters (Windows CRLF files)
static string stripLineEnding(string line) {
    while (!line.empty() &&
           (line.back() == '\r' || line.back() == '\n')) {
        line.pop_back();
    }
    return line;
}

// Trim spaces, tabs and line endings from both ends of a field
static string trimField(const string& value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

// Parse CSV fields, including quoted values
vector<string> parseCSVLine(const string& rawLine) {

    const string line = stripLineEnding(rawLine);

    vector<string> fields;
    string field;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); i++) {

        char ch = line[i];

        if (ch == '"') {

            if (insideQuotes && i + 1 < line.size()
                && line[i + 1] == '"') {

                field += '"';
                i++;
            }
            else {
                insideQuotes = !insideQuotes;
            }
        }
        else if (ch == ',' && !insideQuotes) {

            fields.push_back(trimField(field));
            field.clear();
        }
        else {
            field += ch;
        }
    }

    if (insideQuotes) {
        throw runtime_error("Invalid CSV quotation.");
    }

    fields.push_back(trimField(field));

    return fields;
}

// Convert a value into a safe CSV field
string escapeCSV(const string& value) {

    string result = "\"";

    for (char ch : value) {

        if (ch == '"') {
            result += "\"\"";
        }
        else {
            result += ch;
        }
    }

    result += "\"";

    return result;
}

// Check whether the stored value looks like an Argon2id encoded hash
bool isValidEncodedHash(const string& hash) {

    if (hash.find("$argon2id$v=19$") != 0) {
        return false;
    }

    if (hash.find("YOUR_SALT") != string::npos ||
        hash.find("YOUR_HASH") != string::npos) {
        return false;
    }

    return count(hash.begin(), hash.end(), '$') == 5;
}

// Parse one physical line into one or more user records
vector<vector<string>> parseUserRecords(const string& line) {

    vector<vector<string>> records;

    vector<string> fields = parseCSVLine(line);

    // Normal seven-column record
    if (fields.size() == 7) {
        records.push_back(fields);
        return records;
    }

    // Recover concatenated records.
    // Example: final status "1" followed by next ID "2"
    if (fields.size() > 7 && fields.size() > 6) {

        string boundary = fields[6];

        if (boundary.size() >= 2 &&
            (boundary[0] == '0' || boundary[0] == '1') &&
            all_of(boundary.begin() + 1, boundary.end(),
                   [](unsigned char c) { return isdigit(c); })) {

            vector<string> firstRecord;

            for (int i = 0; i < 6; i++) {
                firstRecord.push_back(fields[i]);
            }

            firstRecord.push_back(string(1, boundary[0]));

            records.push_back(firstRecord);

            vector<string> secondRecord;

            secondRecord.push_back(boundary.substr(1));

            for (size_t i = 7; i < fields.size(); i++) {
                secondRecord.push_back(fields[i]);
            }

            records.push_back(secondRecord);

            return records;
        }
    }

    // Recover older records with unquoted Argon2id hashes
    if (fields.size() > 7) {

        vector<string> corrected;

        for (int i = 0; i < 5; i++) {
            corrected.push_back(fields[i]);
        }

        string hash;

        for (size_t i = 5; i < fields.size() - 1; i++) {

            if (!hash.empty()) {
                hash += ",";
            }

            hash += fields[i];
        }

        corrected.push_back(hash);
        corrected.push_back(fields.back());

        records.push_back(corrected);
    }

    return records;
}

// Constructor
Database::Database(const string& path) {

    filePath = path;

    if (!initializeDatabase()) {
        throw runtime_error("Unable to initialize database.");
    }
}

// Initialize and recover CSV database
bool Database::initializeDatabase() {

    try {

        fs::path path(filePath);

        if (!path.parent_path().empty()) {
            fs::create_directories(path.parent_path());
        }

        if (!fs::exists(path)) {

            ofstream file(filePath);

            if (!file.is_open()) {
                return false;
            }

            file << "userId,fullName,phoneNumber,email,"
                 << "username,passwordHash,isRegistered\n";

            return true;
        }

        // Read existing database
        ifstream input(filePath, ios::binary);

        if (!input.is_open()) {
            return false;
        }

        string content(
            (istreambuf_iterator<char>(input)),
            istreambuf_iterator<char>()
        );

        input.close();

        // Backup original database before recovery
        fs::path backupPath = path;
        backupPath += ".bak";

        if (!fs::exists(backupPath)) {
            fs::copy_file(
                path,
                backupPath,
                fs::copy_options::overwrite_existing
            );
        }

        // Process existing records
        istringstream stream(content);

        string line;

        getline(stream, line); // Skip header

        vector<vector<string>> validRecords;

        while (getline(stream, line)) {

            line = stripLineEnding(line);

            if (line.empty()) {
                continue;
            }

            try {

                vector<vector<string>> records =
                    parseUserRecords(line);

                for (const auto& record : records) {

                    if (record.size() != 7) {
                        continue;
                    }

                    // Keep only records with usable Argon2id hashes
                    if (!isValidEncodedHash(record[5])) {
                        cerr << "Skipping corrupted password record "
                             << "for username: " << record[4] << '\n';
                        continue;
                    }

                    if (record[6] != "0" && record[6] != "1") {
                        continue;
                    }

                    validRecords.push_back(record);
                }
            }
            catch (const exception&) {
                cerr << "Skipping malformed CSV record.\n";
            }
        }

        // Count data lines so we only rewrite when something was dropped/fixed
        // (never wipe a healthy database)
        size_t originalRecordCount = 0;
        {
            istringstream countStream(content);
            string countLine;
            getline(countStream, countLine);
            while (getline(countStream, countLine)) {
                if (!stripLineEnding(countLine).empty()) {
                    originalRecordCount++;
                }
            }
        }

        if (validRecords.size() == originalRecordCount) {
            return true;
        }

        // Rewrite recovered database
        ofstream output(filePath, ios::binary | ios::trunc);

        if (!output.is_open()) {
            return false;
        }

        output << "userId,fullName,phoneNumber,email,"
               << "username,passwordHash,isRegistered\n";

        for (const auto& record : validRecords) {

            for (size_t i = 0; i < record.size(); i++) {

                if (i > 0) {
                    output << ",";
                }

                output << escapeCSV(record[i]);
            }

            output << '\n';
        }

        output.close();

        if (!output.good()) {
            return false;
        }

        cout << "Database recovery completed successfully.\n";

        return true;
    }
    catch (const exception& e) {

        cerr << "Database initialization error: "
             << e.what() << '\n';

        return false;
    }
}

// Check username existence
bool Database::usernameExists(const string& username) {

    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Unable to open database.");
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {

        vector<string> fields = parseCSVLine(line);

        if (fields.size() == 7 && fields[4] == username) {
            return true;
        }
    }

    return false;
}

// Check email existence
bool Database::emailExists(const string& email) {

    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Unable to open database.");
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {

        vector<string> fields = parseCSVLine(line);

        if (fields.size() == 7 && fields[3] == email) {
            return true;
        }
    }

    return false;
}

// Save new user
bool Database::saveUser(
    const Customer& customer,
    const string& passwordHash
) {

    if (usernameExists(customer.getUsername()) ||
        emailExists(customer.getEmail())) {

        return false;
    }

    if (!isValidEncodedHash(passwordHash)) {
        throw runtime_error("Invalid Argon2id password hash.");
    }

    // Ensure existing file ends with a newline
    {
        ifstream check(filePath, ios::binary);

        if (!check.is_open()) {
            throw runtime_error("Unable to read database.");
        }

        check.seekg(0, ios::end);
        streamoff size = check.tellg();

        if (size > 0) {

            check.seekg(-1, ios::end);

            char lastCharacter;
            check.get(lastCharacter);

            check.close();

            if (lastCharacter != '\n' && lastCharacter != '\r') {

                ofstream separator(filePath, ios::app);

                if (!separator.is_open()) {
                    throw runtime_error("Unable to repair CSV line ending.");
                }

                separator << '\n';
            }
        }
    }

    ofstream file(filePath, ios::app);

    if (!file.is_open()) {
        throw runtime_error("Unable to open CSV file for writing.");
    }

    file << customer.getUserID() << ","
         << escapeCSV(customer.getFullName()) << ","
         << escapeCSV(customer.getPhoneNumber()) << ","
         << escapeCSV(customer.getEmail()) << ","
         << escapeCSV(customer.getUsername()) << ","
         << escapeCSV(passwordHash) << ","
         << (customer.getIsRegistered() ? "1" : "0")
         << '\n';

    file.flush();

    if (!file.good()) {
        throw runtime_error("Failed to save user.");
    }

    file.close();

    return true;
}

// Find user using username
bool Database::findUserByUsername(
    const string& username,
    Customer& customer,
    string& passwordHash
) {

    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Unable to open CSV file.");
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {

        vector<string> fields = parseCSVLine(line);

        if (fields.size() != 7) {
            continue;
        }

        if (fields[4] == username) {

            int id = stoi(fields[0]);

            bool registered = (fields[6] == "1");

            customer = Customer(
                id,
                fields[1],
                fields[2],
                fields[3],
                fields[4],
                registered
            );

            passwordHash = fields[5];

            return true;
        }
    }

    return false;
}

// Generate next user ID
int Database::getNextUserId() {

    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Unable to open CSV file.");
    }

    string line;
    int maxId = 0;

    getline(file, line);

    while (getline(file, line)) {

        vector<string> fields = parseCSVLine(line);

        if (fields.empty()) {
            continue;
        }

        try {

            int id = stoi(fields[0]);

            if (id > maxId) {
                maxId = id;
            }
        }
        catch (const exception&) {
            continue;
        }
    }

    return maxId + 1;
}