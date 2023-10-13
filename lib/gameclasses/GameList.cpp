#include "GameList.h"
#include <stdexcept>

// Constructor
GameList::GameList() {

}

// Add a game to the list
void GameList::AddGame(const Game& game) {
    // Add the game to the map using its name as the key
    games.insert(std::make_pair(game.GetGameName(), game));
}

// Get the list of games as a vector
std::vector<Game> GameList::GetGameList() const {
    std::vector<Game> gameList;
    for (const auto& game : games) {
        gameList.push_back(game.second);
    }
    return gameList;
}

// This implementation might changed based on how we save game spec
// Get the game specification for a specific game name
Game GameList::GetGameSpec(const std::string& gameName) const {
    auto it = games.find(gameName);
    if (it != games.end()) {
        return it->second;
    }
    throw std::runtime_error("Game not found"); // Game not found, return an empty string or handle the case as needed
}
