#include "GameState.h"

// Constructor
GameState::GameState() {
    currentRound = 0;
    gameStatus = GameStatus();
}

// Add a player
void GameState::AddPlayer(const std::string& playerName, const Player& player) {
    players[playerName] = player;
}

// Add an audience member
void GameState::AddAudienceMember(const std::string& audienceMemberName, const AudienceMember& audienceMember) {
    audienceMembers[audienceMemberName] = audienceMember;
}

// Get a player
Player GameState::GetPlayer(const std::string& playerName) const {
    auto it = players.find(playerName);
    if (it != players.end()) {
        return it->second;
    }
    // Error
    return Player(" ");
}

// Get an audience member
AudienceMember GameState::GetAudienceMember(const std::string& audienceMemberName) const {
    auto it = audienceMembers.find(audienceMemberName);
    if (it != audienceMembers.end()) {
        return it->second;
    }
    // Error
    return AudienceMember(" ");
}

// Set a game variable
void GameState::SetVariable(const std::string& variableName, const GameVariable& variable) {
    variables[variableName] = variable;
}

// Get a game variable
GameVariable GameState::GetVariable(const std::string& variableName) const {
    auto it = variables.find(variableName);
    if (it != variables.end()) {
        return it->second;
    }
    // Error
    return GameVariable(" ", " ");
}

// Set a game constant
void GameState::SetConstant(const std::string& constantName, const GameConstant& constant) {
    constants[constantName] = constant;
}

// Get a game constant
GameConstant GameState::GetConstant(const std::string& constantName) const {
    auto it = constants.find(constantName);
    if (it != constants.end()) {
        return it->second;
    }
    // Error
    return GameConstant(" ", " ");
}

// Set the current game round
void GameState::SetCurrentRound(int round) {
    currentRound = round;
}

// Get the current game round
int GameState::GetCurrentRound() const {
    return currentRound;
}

// Set the overall game status
void GameState::SetGameStatus(const GameStatus& status) {
    gameStatus = status;
}

// Get the overall game status
GameStatus GameState::GetGameStatus() const {
    return gameStatus;
}
