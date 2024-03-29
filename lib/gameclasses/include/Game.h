#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "Player.h"
#include <map>
#include "AudienceMember.h"
#include "GameVariable.h"
#include "GameConstant.h"
#include "../../executionnodes/include/Tree.h"

class Game {
public:
    enum GameProgress {
        NotStarted,   // Game has not started
        InProgress,   // Game is in progress
        Completed     // Game has completed
    };

    // Constructors
    Game(int newGameId);

    // Getters and Setters
    std::string GetGameName() const;
    void SetGameName(const std::string& name);

    int GetMinPlayers() const;
    void SetMinPlayers(int minPlayers);

    int GetMaxPlayers() const;
    void SetMaxPlayers(int maxPlayers);

    bool IsAudienceEnabled() const;
    void SetAudienceEnabled(bool enabled);

    int GetNumRounds() const;
    void SetNumRounds(int numRounds);

    int GetGameId() const;

    // Set a global game variable
    void AddVariable(const std::string& variableName, const GameVariable& variable);

    // Get a global game variable
    GameVariable GetVariable(const std::string& variableName) const;

    // Get all game variables
    std::vector<GameVariable> GetAllVariables() const;

    // Get all game constants
    std::vector<GameConstant> GetAllConstants() const;

    // Set a global game constant
    void AddConstant(const std::string& constantName, const GameConstant& constant);

    // Get a global game constant
    GameConstant GetConstant(const std::string& constantName) const;

    // Get the current progress of the game
    GameProgress GetGameProgress() const;

    // Set the progress of the game
    void SetGameProgress(GameProgress progress);
    
    void setSource(std::string source);
    std::string getSource();  

    bool operator==(const Game& other) const;

    bool operator<(const Game& other) const;

private: 
    std::string gameName;
    int gameId;
    int minPlayers;
    int maxPlayers;
    bool audienceEnabled;
    int numRounds;
    GameProgress gameProgress;
    std::map<std::string, GameVariable> variables;
    std::map<std::string, GameConstant> constants;
    std::string source;
    
}; 

class ActiveGame{
    public:
    ActiveGame(Game game);
    Game game;
    ExecutionTree gameloop;

};

#endif
