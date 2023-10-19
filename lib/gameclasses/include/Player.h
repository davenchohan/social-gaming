#ifndef PLAYER_H
#define PLAYER_H

#include "User.h"

class Player : public User {
public:
    // Enumeration to represent player states
    enum PlayerState {
        Active,        //  It's their turn
        Eliminated,    // They have been eliminated
        WaitingTurn    // They waiting for their turn
    };

    // Constructor
    Player(const std::string& name, int userId);

    // Set the player's state
    void SetPlayerState(const PlayerState& state);

    // Get the player's state
    PlayerState GetPlayerState() const;

private:
    PlayerState playerState;
};

#endif
