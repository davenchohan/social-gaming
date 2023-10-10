#ifndef GAMELIST_H
#define GAMELIST_H

#include <map>
#include <vector>
#include "Game.h"

class GameList {
public:
    // Constructor
    GameList();

    // Add a game to the list
    void AddGame(const Game& game);

    // Get the list of game names
    std::vector<Game> GetGameList() const;

    // TO DO
    // Get the game specification for a specific game name
    Game GetGameSpec(const std::string& gameName) const;

private:
    std::map<std::string, Game> games; // Map from game name to game
};

#endif
