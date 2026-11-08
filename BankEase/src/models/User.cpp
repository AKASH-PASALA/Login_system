#include "User.h"
#include <iostream>

using namespace std;

User::User() : userId(0), fullName(""), phoneNumber(""), email("") 
{
    
}
User::User(int id, string name, string phone, string email)
    : userId(id), fullName(name), phoneNumber(phone), email(email)
{
    
}
int User::getUserID() const {
    return userId;
}
string User::getFullName() const {
    return fullName;
}
string User::getPhoneNumber() const {
    return phoneNumber;
}
string User::getEmail() const {
    return email;
}
void User::setFullName(string name) {
    fullName = name;
}

void User::setPhoneNumber(string phone) {
    phoneNumber = phone;
}

void User::setEmail(string email) {
    this->email = email;
}
void User::displayUserInfo() const{
    cout << "User ID: " << userId << endl;
    cout << "Full Name: " << fullName << endl;
    cout << "Phone Number: " << phoneNumber << endl;
    cout << "Email: " << email << endl;
}