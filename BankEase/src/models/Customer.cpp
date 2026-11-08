#include "Customer.h"
#include <iostream>
using namespace std;
Customer::Customer() : User(), username(""), isRegistered(false) 
{

}
Customer::Customer(int id, std::string name, std::string phone, std::string email, std::string username, bool isRegistered) : User(id, name, phone, email), username(username), isRegistered(isRegistered)
{

}
std::string Customer::getUsername() const{
    return username;
}
bool Customer::getIsRegistered() const{
    return isRegistered;
}
void Customer::setUsername(string username){
    this->username = username;
}
void Customer::setIsRegistered(bool isRegistered){
    this->isRegistered = isRegistered;
}
void Customer::displayCustomerInfo() const{
    User::displayUserInfo();
    cout << "Username: " << getUsername() << endl;
    cout << "Is Registered: " << (getIsRegistered() ? "Yes" : "No") << endl;
}
