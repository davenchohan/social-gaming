#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <map>
#include "Player.h"
#include "AudienceMember.h"
#include "GameVariable.h"
#include "GameConstant.h"
#include "GameStatus.h"

class GameState {
public:
    // Constructor
    GameState();

    // Add a player state
    void AddPlayer(const std::string& playerName, const Player& Player);

    // Add an audience member state
    void AddAudienceMember(const std::string& audienceMemberName, const AudienceMember& audienceMember);

    // Get a player's state
    Player GetPlayer(const std::string& playerName) const;

    // Get an audience member's state
    AudienceMember GetAudienceMember(const std::string& audienceMemberName) const;

    // Set a global game variable
    void SetVariable(const std::string& variableName, const GameVariable& variable);

    // Get a global game variable
    GameVariable GetVariable(const std::string& variableName) const;

    // Set a global game constant
    void SetConstant(const std::string& constantName, const GameConstant& constant);

    // Get a global game constant
    GameConstant GetConstant(const std::string& constantName) const;

    // Set the current game round
    void SetCurrentRound(int round);

    // Get the current game round
    int GetCurrentRound() const;

    // Set the overall game status
    void SetGameStatus(const GameStatus& status);

    // Get the overall game status
    GameStatus GetGameStatus() const;

private:
    std::map<std::string, Player> players;
    std::map<std::string, AudienceMember> audienceMembers;
    std::map<std::string, GameVariable> variables;
    std::map<std::string, GameConstant> constants;
    int currentRound;
    GameStatus gameStatus;
};

#endif
