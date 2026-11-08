#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <string>

class PasswordManager {
public:

    // Generate a secure random salt
    static std::string generateSalt();

    // Hash password using Argon2id
    static std::string hashPassword(const std::string& password);

    // Verify password against stored hash
    static bool verifyPassword(
        const std::string& password,
        const std::string& encodedHash
    );
};

#endif