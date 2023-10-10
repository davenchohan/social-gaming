#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H

#include <string>

class GameVariable {
public:
    // Constructor
    GameVariable(const std::string& name, const std::string& value);

    // Get the value of the variable
    std::string GetVariableValue() const;

private:
    std::string variableName;
    std::string variableValue;
};

#endif
