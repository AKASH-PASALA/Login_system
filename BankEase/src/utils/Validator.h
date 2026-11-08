#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator {
public:
    static bool isValidName(const std::string& name);

    static bool isValidEmail(const std::string& email);

    static bool isValidPhoneNumber(const std::string& phoneNumber);

    static bool isValidUsername(const std::string& username);

    static bool isValidPassword(const std::string& password);

    static bool isPasswordMatching(
        const std::string& password,
        const std::string& confirmPassword
    );
};

#endif