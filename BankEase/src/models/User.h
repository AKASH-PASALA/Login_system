#include<string>
#ifndef USER_H
#define USER_H

using namespace std;
class User {
    private:
        int userId;
        string fullName;
        string phoneNumber;
        string email;
    public:
        User();
        User(int id, std::string name, std::string phone, std::string email);
        int getUserID() const;
        string getFullName() const;
        string getPhoneNumber() const;
        string getEmail() const;
        void setFullName(string name);
        void setPhoneNumber(string phone);
        void setEmail(string email);
        void displayUserInfo() const;
    };
#endif