#include "DataClasses.h"

JoinGame::JoinGame(const std::string& userID, const std::string& gameID)
    : userID(userID), gameID(gameID) {}

std::string JoinGame::toJSON() {
    return "{\"UserID\":\"" + userID + "\",\"GameID\":\"" + gameID + "\"}";
}

CreateGame::CreateGame(const std::string& userID, const std::string& gameID, const std::string& config)
    : userID(userID), gameID(gameID), config(config) {}

std::string CreateGame::toJSON() {
    return "{\"UserID\":\"" + userID + "\",\"GameID\":\"" + gameID + "\",\"Config\":\"" + config + "\"}";
}

// CONSIDER: potentially chagne data type of gameID that makes more sense rather than just using string (but we can still use it if we want to use ID that has characters etc)
GetGame::GetGame(const std::string& gameID) : gameID(gameID) {}

std::string GetGame::toJSON() {
    return "{\"GameID\":\"" + gameID + "\"}";
}

// Temporary implementation to test get game
GetGameName::GetGameName(const std::string& gameName) : gameName(gameName) {}

std::string GetGameName::toJSON() {
    return gameName;
}