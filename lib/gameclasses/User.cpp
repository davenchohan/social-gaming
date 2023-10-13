#include "User.h"

// Constructor
User::User(const std::string& name, int userId) : userName(name), uniqueUserId(userId) {

}

// Get the user's name
std::string User::GetName() const {
    return userName;
}

// Get the user's unique ID
int User::GetUserId() const {
    return uniqueUserId;
}
