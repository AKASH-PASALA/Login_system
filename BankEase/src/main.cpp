#include <iostream>
#include <string>
#include <limits>

#include "authentication/Authentication.h"

using namespace std;

// Display main menu
void displayMainMenu() {
    cout << "\n====================================\n";
    cout << "          WELCOME TO BANKEASE       \n";
    cout << "====================================\n";
    cout << "1. Sign Up\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";
}

// Display customer dashboard
void displayDashboard(Authentication& auth) {

    int choice;

    while (auth.isLoggedIn()) {

        Customer user = auth.getCurrentUser();

        cout << "\n====================================\n";
        cout << "           BANKEASE DASHBOARD       \n";
        cout << "====================================\n";

        cout << "Welcome, " << user.getFullName() << "!\n";
        cout << "User ID: " << user.getUserID() << "\n";
        cout << "Username: " << user.getUsername() << "\n";

        cout << "------------------------------------\n";
        cout << "1. View Profile\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            case 1:
                cout << "\n========== CUSTOMER PROFILE ==========\n";
                user.displayCustomerInfo();
                cout << "======================================\n";
                break;

            case 2:
                auth.logout();
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

// Main function
int main() {

    try {

        Authentication auth;

        int choice;

        while (true) {

            displayMainMenu();

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {

                // SIGNUP
                case 1: {

                    string fullName;
                    string phoneNumber;
                    string email;
                    string username;
                    string password;
                    string confirmPassword;

                    cout << "\n========== USER REGISTRATION ==========\n";

                    cout << "Enter Full Name: ";
                    getline(cin, fullName);

                    cout << "Enter Phone Number: ";
                    getline(cin, phoneNumber);

                    cout << "Enter Email: ";
                    getline(cin, email);

                    cout << "Enter Username: ";
                    getline(cin, username);

                    cout << "Enter Password: ";
                    getline(cin, password);

                    cout << "Confirm Password: ";
                    getline(cin, confirmPassword);

                    cout << "\nProcessing registration...\n";

                    auth.signup(
                        fullName,
                        phoneNumber,
                        email,
                        username,
                        password,
                        confirmPassword
                    );

                    break;
                }

                // LOGIN
                case 2: {

                    string username;
                    string password;

                    cout << "\n============== USER LOGIN ==============\n";

                    cout << "Enter Username: ";
                    getline(cin, username);

                    cout << "Enter Password: ";
                    getline(cin, password);

                    if (auth.login(username, password)) {
                        displayDashboard(auth);
                    }

                    break;
                }

                // EXIT
                case 3:

                    cout << "\nThank you for using BankEase!\n";
                    cout << "Application closed successfully.\n";

                    return 0;

                default:

                    cout << "Invalid choice. Please try again.\n";
            }
        }

    }
    catch (const exception& e) {

        cerr << "\nApplication Error: " << e.what() << '\n';

        return 1;
    }

    return 0;
}