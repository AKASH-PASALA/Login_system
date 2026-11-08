#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <string>

class Customer : public User {

private:
    std::string username;
    bool isRegistered;

public:
    Customer();

    Customer(
        int id,
        std::string name,
        std::string phone,
        std::string email,
        std::string username,
        bool isRegistered
    );

    std::string getUsername() const;
    bool getIsRegistered() const;

    void setUsername(std::string username);
    void setIsRegistered(bool isRegistered);

    void displayCustomerInfo() const;
};

#endif