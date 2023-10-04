#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H

#include <string>

class GameVariable {
public:
    // Constructors
    GameVariable();
    GameVariable(const std::string& stringValue);
    // Getter
    std::string GetVariable() const;

private:
    std::string data;
};

#endif
