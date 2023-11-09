#include "Game.h"
#include <stdexcept>

// Default constructor
Game::Game(int newGameId) : gameId(newGameId) {
    gameName = "Game";
    minPlayers = 2; // Default minimum players
    maxPlayers = 4; // Default maximum players
    audienceEnabled = true; // Default audience enabled
    numRounds = 3; // Default number of rounds
    gameProgress = NotStarted; // Default game progress
}

// Getters and Setters
std::string Game::GetGameName() const {
    return gameName;
}

void Game::SetGameName(const std::string& name) {
    gameName = name;
}

int Game::GetMinPlayers() const {
    return minPlayers;
}

void Game::SetMinPlayers(int min) {
    minPlayers = min;
}

int Game::GetMaxPlayers() const {
    return maxPlayers;
}

void Game::SetMaxPlayers(int max) {
    maxPlayers = max;
}

bool Game::IsAudienceEnabled() const {
    return audienceEnabled;
}

void Game::SetAudienceEnabled(bool enabled) {
    audienceEnabled = enabled;
}

int Game::GetNumRounds() const {
    return numRounds;
}

void Game::SetNumRounds(int rounds) {
    numRounds = rounds;
}

// Set a game variable
void Game::AddVariable(const std::string& variableName, const GameVariable& variable) {
    variables.insert(std::make_pair(variableName, variable));
}

// Get a game variable
GameVariable Game::GetVariable(const std::string& variableName) const {
    auto it = variables.find(variableName);
    if (it != variables.end()) {
        return it->second;
    }
    // Error
    throw std::runtime_error("Variable not found");
}

// Set a game constant
void Game::AddConstant(const std::string& constantName, const GameConstant& constant) {
    constants.insert(std::make_pair(constantName, constant));
}

// Get a game constant
GameConstant Game::GetConstant(const std::string& constantName) const {
    auto it = constants.find(constantName);
    if (it != constants.end()) {
        return it->second;
    }
    // Error
    throw std::runtime_error("Constant not found");
}

// Get the game ID
int Game::GetGameId() const {
    return gameId;
}

// Set the progress of the game
void Game::SetGameProgress(Game::GameProgress progress) {
    gameProgress = progress;
}

// Get the current progress of the game
Game::GameProgress Game::GetGameProgress() const {
    return gameProgress;
}
void Game::setSource(std::string source_ )
{
    source = source_;

}
std::string Game::getSource(){
    return source;
}

bool Game::operator==(const Game& other) const {
    return this->GetGameId() == other.GetGameId();
}

bool Game::operator<(const Game& other) const {
    return this->GetGameId() < other.GetGameId();
}