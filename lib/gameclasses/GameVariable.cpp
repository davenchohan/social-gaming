#include "GameVariable.h"

// Constructors
GameVariable::GameVariable() : data(0), variableName("") {}

GameVariable::GameVariable(const std::string& name, const int& intValue) : data(intValue), variableName(name) {}

GameVariable::GameVariable(const std::string& name, const double& doubleValue) : data(doubleValue), variableName(name) {}

GameVariable::GameVariable(const std::string& name, const std::string& stringValue) : data(stringValue), variableName(name) {}

// Type retrieval functions
bool GameVariable::IsInt() const {
    return data.index() == 0;
}

bool GameVariable::IsDouble() const {
    return data.index() == 1;
}

bool GameVariable::IsString() const {
    return data.index() == 2;
}

// Overloaded comparison operators
bool GameVariable::operator==(const GameVariable& other) const {
    return this->data == other.data;
}

bool GameVariable::operator<(const GameVariable& other) const {
    return this->data < other.data;
}

std::string GameVariable::GetName() const {
    return variableName;
}
