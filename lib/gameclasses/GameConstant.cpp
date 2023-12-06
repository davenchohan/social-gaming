#include "GameConstant.h"

// Constructor
GameConstant::GameConstant(const std::string& name, const std::string& value) {
    constantName = name;
    constantValue = value;
}

// Get the value of the constant
std::string GameConstant::GetConstantValue() const {
    return constantValue;
}

// Overloaded comparison operators
bool GameConstant::operator==(const GameConstant& other) const {
    return this->GetConstantValue() == other.GetConstantValue();
}

bool GameConstant::operator<(const GameConstant& other) const {
    return this->GetConstantValue() < other.GetConstantValue();
}

std::string GameConstant::GetName() const {
    return constantName;
}

