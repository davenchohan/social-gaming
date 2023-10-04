#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <string>
#include <vector>
#include "Player.h"

class GameStatus {
public:
    enum GameProgress {
        NotStarted,   // Game has not started
        InProgress,   // Game is in progress
        Completed     // Game has completed
    };

    // Constructors
    GameStatus();

    // Getters and Setters
    std::string GetGameName() const;
    void SetGameName(const std::string& name);

    int GetMinPlayers() const;
    void SetMinPlayers(int minPlayers);

    int GetMaxPlayers() const;
    void SetMaxPlayers(int maxPlayers);

    bool IsAudienceEnabled() const;
    void SetAudienceEnabled(bool enabled);

    int GetNumRounds() const;
    void SetNumRounds(int numRounds);

private:
    std::string gameName;
    int minPlayers;
    int maxPlayers;
    bool audienceEnabled;
    int numRounds;
    GameProgress gameProgress;

};

#endif
