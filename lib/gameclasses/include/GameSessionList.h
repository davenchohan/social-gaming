#ifndef GAMESESSIONLIST_H
#define GAMESESSIONLIST_H

#include <map>
#include "GameSessionHandler.h"
#include "RoomCodeGenerator.h"

// TODO need to replace DoesSessionExist to check via roomCode rather than sessionId
class GameSessionList {
public:
    // Constructor
    GameSessionList();

    // Add a game session handler to the list
    void AddGameSessionHandler(const std::string& sessionId, GameSessionHandler& gameSessionHandler);

    // Get the list of game session handlers
    std::map<std::string, GameSessionHandler> GetGameSessionHandlers() const;

    // Check if a session with a given sessionId exists
    bool DoesSessionExist(const std::string& sessionId) const;

    // Get a specific game session handler
    GameSessionHandler GetGameSessionHandler(const std::string& sessionId) const;

private:
    std::map<std::string, GameSessionHandler> sessionHandlerDB;
    std::map<std::string, GameSessionHandler> roomCodes;
    bool doesCodeExist(std::string enteredRoomCode) const;
    RoomCodeGenerator roomCodeGenerator;
};

#endif
