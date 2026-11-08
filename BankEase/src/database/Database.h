#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../models/Customer.h"

class Database {

private:
    std::string filePath;

    bool initializeDatabase();

public:
    // Constructor
    Database(const std::string& path = "data/users.csv");

    // Check whether username already exists
    bool usernameExists(const std::string& username);

    // Check whether email already exists
    bool emailExists(const std::string& email);

    // Save a new customer
    bool saveUser(const Customer& customer,
                  const std::string& passwordHash);

    // Retrieve customer using username
    bool findUserByUsername(
        const std::string& username,
        Customer& customer,
        std::string& passwordHash
    );

    // Generate the next user ID
    int getNextUserId();
};

#endif