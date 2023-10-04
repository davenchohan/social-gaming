#include "GameVariable.h"

// Constructor
GameVariable::GameVariable(const std::string& name, const std::string& value) {
    variableName = name;
    variableValue = value;
}

// Get the value of the Variable
std::string GameVariable::GetVariableValue() const {
    return variableValue;
}
