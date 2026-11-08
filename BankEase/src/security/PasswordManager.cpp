#include "PasswordManager.h"
#include <argon2.h>
#include <random>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// 1. Generate a cryptographically secure random salt
string PasswordManager::generateSalt() {

    random_device rd;

    vector<unsigned char> salt(16);

    for (auto& byte : salt) {
        byte = static_cast<unsigned char>(rd() & 0xFF);
    }

    // Convert salt bytes into hexadecimal string
    const char hex[] = "0123456789abcdef";

    string result;

    for (unsigned char byte : salt) {
        result += hex[byte >> 4];
        result += hex[byte & 0x0F];
    }

    return result;
}


// 2. Hash password using Argon2id
string PasswordManager::hashPassword(const string& password) {

    string salt = generateSalt();

    // Argon2id parameters
    const uint32_t iterations = 3;
    const uint32_t memoryCost = 65536;
    const uint32_t parallelism = 1;
    const uint32_t hashLength = 32;

    // Buffer for encoded hash
    vector<char> encodedHash(256);

    int result = argon2id_hash_encoded(
        iterations,
        memoryCost,
        parallelism,
        password.data(),
        password.size(),
        salt.data(),
        salt.size(),
        hashLength,
        encodedHash.data(),
        encodedHash.size()
    );

    if (result != ARGON2_OK) {
        throw runtime_error(
            string("Password hashing failed: ") + argon2_error_message(result)
        );
    }

    return string(encodedHash.data());
}


// 3. Verify password using Argon2id
bool PasswordManager::verifyPassword(
    const string& password,
    const string& encodedHash
) {

    int result = argon2id_verify(
        encodedHash.c_str(),
        password.data(),
        password.size()
    );

    if (result == ARGON2_OK) {
        return true;
    }

    if (result == ARGON2_VERIFY_MISMATCH) {
        return false;
    }

    throw runtime_error(
        string("Password verification failed: ") + argon2_error_message(result)
    );
}