#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <string>
#include <vector>
#include "Player.h" // Include the Player class if needed

class GameStatus {
public:
    enum GameProgress {
        NotStarted,   // Game has not started
        InProgress,   // Game is in progress
        Completed     // Game has completed
    };

    // Constructors
    GameStatus();

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

    // Add, and retrieve players
    void AddPlayer(const Player& player);
    std::vector<Player> GetPlayers() const;

    // Add, and retrieve audience members
    void AddAudienceMember(const std::string& audienceMemberName);
    std::vector<std::string> GetAudienceMembers() const;

private:
    std::string gameName;
    int minPlayers;
    int maxPlayers;
    bool audienceEnabled;
    int numRounds;
    std::vector<Player> players;
    std::vector<std::string> audienceMembers;
    GameProgress gameProgress;

};

#endif
