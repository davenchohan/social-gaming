#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    // Constructor
    Player(const std::string& name);

    // Set a state value
    void SetStateValue(const std::string& value);

    // Get the value of a state
    std::string GetStateValue() const;

private:
    std::string playerState;
    std::string playerName;
};

#endif
