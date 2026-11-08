#include "Authentication.h"

#include "../utils/Validator.h"
#include "../security/PasswordManager.h"

#include <iostream>
#include <stdexcept>

using namespace std;

// Constructor
Authentication::Authentication()
    : database("data/users.csv"),
      currentUser(),
      loggedIn(false) {
}

// Signup
bool Authentication::signup(
    const string& fullName,
    const string& phoneNumber,
    const string& email,
    const string& username,
    const string& password,
    const string& confirmPassword
) {

    // Validate full name
    if (!Validator::isValidName(fullName)) {
        cout << "Invalid full name.\n";
        return false;
    }

    // Validate phone number
    if (!Validator::isValidPhoneNumber(phoneNumber)) {
        cout << "Invalid phone number.\n";
        return false;
    }

    // Validate email
    if (!Validator::isValidEmail(email)) {
        cout << "Invalid email address.\n";
        return false;
    }

    // Validate username
    if (!Validator::isValidUsername(username)) {
        cout << "Username must contain 4-20 letters, numbers or underscores.\n";
        return false;
    }

    // Validate password
    if (!Validator::isValidPassword(password)) {
        cout << "Password must contain at least 8 characters, "
             << "uppercase, lowercase, digit and special character.\n";
        return false;
    }

    // Confirm password
    if (!Validator::isPasswordMatching(password, confirmPassword)) {
        cout << "Passwords do not match.\n";
        return false;
    }

    // Check duplicate username
    if (database.usernameExists(username)) {
        cout << "Username already exists.\n";
        return false;
    }

    // Check duplicate email
    if (database.emailExists(email)) {
        cout << "Email already registered.\n";
        return false;
    }

    // Generate password hash
    string passwordHash = PasswordManager::hashPassword(password);

    // Generate user ID
    int userId = database.getNextUserId();

    // Create customer object
    Customer newCustomer(
        userId,
        fullName,
        phoneNumber,
        email,
        username,
        true
    );

    // Save customer to CSV
    if (database.saveUser(newCustomer, passwordHash)) {
        cout << "Registration successful!\n";
        cout << "Your User ID: " << userId << "\n";
        return true;
    }

    cout << "Registration failed.\n";
    return false;
}

// Login
bool Authentication::login(
    const string& username,
    const string& password
) {

    if (loggedIn) {
        cout << "A user is already logged in. Please logout first.\n";
        return false;
    }

    Customer foundCustomer;
    string storedHash;

    // Find user by username
    if (!database.findUserByUsername(
            username,
            foundCustomer,
            storedHash
        )) {

        cout << "Invalid username or password.\n";
        return false;
    }

    // Verify password
    if (!PasswordManager::verifyPassword(password, storedHash)) {
        cout << "Invalid username or password.\n";
        return false;
    }

    // Check registration status
    if (!foundCustomer.getIsRegistered()) {
        cout << "This account is not active.\n";
        return false;
    }

    // Start session
    currentUser = foundCustomer;
    loggedIn = true;

    cout << "\nLogin successful!\n";
    cout << "Welcome, " << currentUser.getFullName() << "!\n";

    return true;
}

// Logout
void Authentication::logout() {

    if (!loggedIn) {
        cout << "No user is currently logged in.\n";
        return;
    }

    cout << "Goodbye, " << currentUser.getFullName() << "!\n";

    currentUser = Customer();
    loggedIn = false;

    cout << "Logout successful!\n";
}

// Check login status
bool Authentication::isLoggedIn() const {
    return loggedIn;
}

// Get current user
Customer Authentication::getCurrentUser() const {
    return currentUser;
}