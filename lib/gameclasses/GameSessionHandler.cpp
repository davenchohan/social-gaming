#include "GameSessionHandler.h"
#include <stdexcept>

// Constructor
GameSessionHandler::GameSessionHandler(int id, Game& gameObj, Player& gameHost) : sessionId(id), game(gameObj), host(gameHost) {
}

// Add a player
void GameSessionHandler::AddPlayer(const std::string& playerName, const Player& player) {
    players.insert(std::make_pair(playerName, player));
}

// Add an audience member
void GameSessionHandler::AddAudienceMember(const std::string& audienceMemberName, const AudienceMember& audienceMember) {
    audienceMembers.insert(std::make_pair(audienceMemberName, audienceMember));
}

// Get a player
Player GameSessionHandler::GetPlayer(const std::string& playerName) const {
    auto it = players.find(playerName);
    if (it != players.end()) {
        return it->second;
    }
    // Error
    throw std::runtime_error("Player not found");
}

// Get an audience member
AudienceMember GameSessionHandler::GetAudienceMember(const std::string& audienceMemberName) const {
    auto it = audienceMembers.find(audienceMemberName);
    if (it != audienceMembers.end()) {
        return it->second;
    }
    // Error
    throw std::runtime_error("Audience Member not found");
}

// Remove a player
void GameSessionHandler::RemovePlayer(const std::string& playerName) {
    players.erase(playerName);
}

// Remove an audience member
void GameSessionHandler::RemoveAudienceMember(const std::string& audienceMemberName) {
    audienceMembers.erase(audienceMemberName);
}

// Set the current game round
void GameSessionHandler::SetCurrentRound(int round) {
    currentRound = round;
}

// Get the current game round
int GameSessionHandler::GetCurrentRound() const {
    return currentRound;
}

// Get the overall game status
Game GameSessionHandler::GetGame() const {
    return game;
}

// Start the game
void GameSessionHandler::startSession() {
    // Start the game here
}

// End the game
void GameSessionHandler::endSession() {
    // End the game here
}

// Get the session ID
int GameSessionHandler::GetSessionId() const {
    return sessionId;
}

Player GameSessionHandler::GetHost() const {
    return host;
}

void GameSessionHandler::SetHost(Player& gameHost) {
    host = gameHost;
}