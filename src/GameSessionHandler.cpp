#include "GameSessionHandler.h"

// Constructor
GameSessionHandler::GameSessionHandler(int id, Game& gameObj) : game(gameObj), sessionId(sessionId) {
}

// Add a player
void GameSessionHandler::AddPlayer(const std::string& playerName, const Player& player) {
    players[playerName] = player;
}

// Add an audience member
void GameSessionHandler::AddAudienceMember(const std::string& audienceMemberName, const AudienceMember& audienceMember) {
    audienceMembers[audienceMemberName] = audienceMember;
}

// Get a player
Player GameSessionHandler::GetPlayer(const std::string& playerName) const {
    auto it = players.find(playerName);
    if (it != players.end()) {
        return it->second;
    }
    // Error
    return Player("ERROR", -1);
}

// Get an audience member
AudienceMember GameSessionHandler::GetAudienceMember(const std::string& audienceMemberName) const {
    auto it = audienceMembers.find(audienceMemberName);
    if (it != audienceMembers.end()) {
        return it->second;
    }
    // Error
    return AudienceMember("ERROR", -1);
}

// Remove a player
void GameSessionHandler::RemovePlayer(const std::string& playerName) {
    // Find and remove the player with the specified name
    players.erase(playerName);
}

// Remove an audience member
void GameSessionHandler::RemoveAudienceMember(const std::string& audienceMemberName) {
    // Find and remove the player with the specified name
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