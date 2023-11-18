#include "GameSessionList.h"
#include "RoomCodeGenerator.h"
#include "GameSessionHandler.h"
#include <stdexcept>

// Constructor
GameSessionList::GameSessionList() {
    roomCodeGenerator = RoomCodeGenerator();
}

// Add a game session handler to the list
void GameSessionList::AddGameSessionHandler(const std::string& sessionId, GameSessionHandler& gameSessionHandler) {
    std::string newRoomCode;
    do {
        newRoomCode = roomCodeGenerator.generateRoomCode();
    } while (doesCodeExist(newRoomCode));
    gameSessionHandler.SetRoomCode(newRoomCode);
    roomCodes.insert(std::make_pair(newRoomCode, gameSessionHandler));
    sessionHandlerDB.insert(std::make_pair(sessionId, gameSessionHandler));
}

// Get the list of game session handlers
std::map<std::string, GameSessionHandler> GameSessionList::GetGameSessionHandlers() const {
    return sessionHandlerDB;
}

bool GameSessionList::DoesSessionExist(const std::string& sessionId) const {
    return sessionHandlerDB.find(sessionId) != sessionHandlerDB.end();
}

// Get a specific game session handler
GameSessionHandler GameSessionList::GetGameSessionHandler(const std::string& sessionId) const {
    auto it = sessionHandlerDB.find(sessionId);
    if (it != sessionHandlerDB.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Session not found");
    }
}

bool GameSessionList::doesCodeExist(std::string enteredRoomCode) const {
    auto iterator = roomCodes.find(enteredRoomCode);
    if (iterator != roomCodes.end()) {
        return true;
    } else {
        return false;
    }
}