#include "GameStatus.h"

// Default constructor
GameStatus::GameStatus() {
    gameName = "Game";
    minPlayers = 2; // Default minimum players
    maxPlayers = 4; // Default maximum players
    audienceEnabled = true; // Default audience enabled
    numRounds = 3; // Default number of rounds
    gameProgress = NotStarted; // Default game progress
}

// Getters and Setters
std::string GameStatus::GetGameName() const {
    return gameName;
}

void GameStatus::SetGameName(const std::string& name) {
    gameName = name;
}

int GameStatus::GetMinPlayers() const {
    return minPlayers;
}

void GameStatus::SetMinPlayers(int min) {
    minPlayers = min;
}

int GameStatus::GetMaxPlayers() const {
    return maxPlayers;
}

void GameStatus::SetMaxPlayers(int max) {
    maxPlayers = max;
}

bool GameStatus::IsAudienceEnabled() const {
    return audienceEnabled;
}

void GameStatus::SetAudienceEnabled(bool enabled) {
    audienceEnabled = enabled;
}

int GameStatus::GetNumRounds() const {
    return numRounds;
}

void GameStatus::SetNumRounds(int rounds) {
    numRounds = rounds;
}