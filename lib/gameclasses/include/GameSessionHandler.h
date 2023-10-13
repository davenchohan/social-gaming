#ifndef GAMESESSIONHANDLER_H
#define GAMESESSIONHANDLER_H

#include <map>
#include "Player.h"
#include "AudienceMember.h"
#include "GameVariable.h"
#include "GameConstant.h"
#include "Game.h"

// TODO: HANDLE USER INPUT, SENDING GAMESTATE TO FRONT END, END THE SESSION
class GameSessionHandler {
public:
    // Constructor
    GameSessionHandler(int id, Game& gameObj);

    // Add a player
    void AddPlayer(const std::string& playerName, const Player& Player);

    // Remove a player
    void RemovePlayer(const std::string& playerName);

    // Add an audience member
    void AddAudienceMember(const std::string& audienceMemberName, const AudienceMember& audienceMember);

    // Get a player
    Player GetPlayer(const std::string& playerName) const;

    // Get an audience member
    AudienceMember GetAudienceMember(const std::string& audienceMemberName) const;

    // Remove an audience member
    void RemoveAudienceMember(const std::string& audienceMemberName);

    // Set the current game round
    void SetCurrentRound(int round);

    // Get the current game round
    int GetCurrentRound() const;

    // Get the game specification
    Game GetGame() const;

    void startSession();

    void endSession();

    void getUserInput();

    int GetSessionId() const;

private:
    std::map<std::string, Player> players;
    std::map<std::string, AudienceMember> audienceMembers;
    int currentRound;
    int sessionId;
    Game game;
};

#endif
