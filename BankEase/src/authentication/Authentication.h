#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include "../models/Customer.h"
#include "../database/Database.h"

class Authentication {

private:
    Database database;
    Customer currentUser;
    bool loggedIn;

public:
    // Constructor
    Authentication();

    // Register a new user
    bool signup(
        const std::string& fullName,
        const std::string& phoneNumber,
        const std::string& email,
        const std::string& username,
        const std::string& password,
        const std::string& confirmPassword
    );

    // Authenticate existing user
    bool login(
        const std::string& username,
        const std::string& password
    );

    // Logout current user
    void logout();

    // Check login status
    bool isLoggedIn() const;

    // Get currently logged-in customer
    Customer getCurrentUser() const;
};

#endif