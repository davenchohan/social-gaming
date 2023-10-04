#include "Player.h"

// Constructor
Player::Player(const std::string& name) : playerName(name) {
    
}

// Set a state value
void Player::SetStateValue(const std::string& value) {
    playerState = value;
}

// Get the value of a state
std::string Player::GetStateValue() const {
    return playerState;
}
