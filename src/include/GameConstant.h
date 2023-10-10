#ifndef GAMECONSTANT_H
#define GAMECONSTANT_H

#include <string>

class GameConstant {
public:
    // Constructor
    GameConstant(const std::string& name, const std::string& value);

    // Get the value of the constant
    std::string GetConstantValue() const;

private:
    std::string constantName;
    std::string constantValue;
};

#endif
