#include "GameVariable.h"

// Default constructor
GameVariable::GameVariable() {
    data = std::string(""); // Default to an empty string
}

// Constructor for string data type
GameVariable::GameVariable(const std::string& stringValue) {
    data = stringValue;
}

// Get data
std::string GameVariable::GetVariable() const {
    return data;
}
