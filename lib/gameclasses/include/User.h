#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    // Constructor
    User(const std::string& name, int userId);

    // Get the user's name
    std::string GetName() const;

    // Get the user's unique ID
    int GetUserId() const;

private:
    std::string userName;
    int uniqueUserId;
};

#endif
