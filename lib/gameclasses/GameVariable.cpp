#include "GameVariable.h"

// Constructors
GameVariable::GameVariable(const int &id) : id(id) {} 
GameVariable::GameVariable(const std::string& name, const int id, const int& intValue) : data(intValue), id(id), variableName(name) {}

GameVariable::GameVariable(const std::string& name, const int id, const double& doubleValue) : data(doubleValue), id(id), variableName(name) {}

GameVariable::GameVariable(const std::string& name, const int id, const std::string& stringValue) : data(stringValue), id(id), variableName(name) {}

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

int GameVariable::GetVariableId() const {
    return id;
}

// Overloaded comparison operators
bool GameVariable::operator==(const GameVariable& other) const {
    return this->GetVariableId() == other.GetVariableId();
}

bool GameVariable::operator<(const GameVariable& other) const {
    return this->GetVariableId() < other.GetVariableId();
}

std::string GameVariable::GetName() const {
    return variableName;
}
