#include "Player.h"

// Constructor
Player::Player(const std::string& name, int userId) : User(name, userId), playerState(WaitingTurn) {
}

// Set player-specific state
void Player::SetPlayerState(const PlayerState& state) {
    playerState = state;
}

// Get the player's state
Player::PlayerState Player::GetPlayerState() const {
    return playerState;
}