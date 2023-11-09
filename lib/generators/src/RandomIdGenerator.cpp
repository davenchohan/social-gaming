#include "RandomIdGenerator.h"
#include <string>
#include <ctime>

// Generates a unique ID
int RandomIdGenerator::generateUniqueId() {
    std::time_t result = std::time(nullptr);
    std::string uniqueID = std::to_string(result);
    int randomNum = std::rand();
    uniqueID += std::to_string(randomNum);

    std::hash<std::string> hasher;
    return static_cast<int>(hasher(uniqueID));
}