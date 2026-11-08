#include "Validator.h"
#include <regex>
#include <cctype>

// 1. Validate full name
bool Validator::isValidName(const std::string& name) {

    std::regex pattern("^[A-Za-z]+( [A-Za-z]+)*$");

    return std::regex_match(name, pattern);
}

// 2. Validate email
bool Validator::isValidEmail(const std::string& email) {

    std::regex pattern(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
    );

    return std::regex_match(email, pattern);
}

// 3. Validate phone number
bool Validator::isValidPhoneNumber(const std::string& phoneNumber) {

    std::regex pattern("^[0-9]{10}$");

    return std::regex_match(phoneNumber, pattern);
}

// 4. Validate username
bool Validator::isValidUsername(const std::string& username) {

    std::regex pattern("^[A-Za-z0-9_]{4,20}$");

    return std::regex_match(username, pattern);
}

// 5. Validate password strength
bool Validator::isValidPassword(const std::string& password) {

    if (password.length() < 8)
        return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char ch : password) {

        unsigned char c = static_cast<unsigned char>(ch);

        if (std::isupper(c))
            hasUpper = true;

        else if (std::islower(c))
            hasLower = true;

        else if (std::isdigit(c))
            hasDigit = true;

        else
            hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// 6. Compare password and confirm password
bool Validator::isPasswordMatching(
    const std::string& password,
    const std::string& confirmPassword
) {

    return password == confirmPassword;
}