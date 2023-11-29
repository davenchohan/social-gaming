#include "GamesDatabase.h"
#include <stdexcept>

// // Constructors
// namespace GamesDatabase {
	// GamesDatabase(std::map<std::string, ts::Tree> gamesDatabase);
	// GamesDatabase(Game game, ts::Tree config) {
	// 	// initialize
	// 	std::map<std::string, ts::Tree> gameDatabase = {{game, config}};
	// 	allGames = game;
	// 	defaultConfigurations = config;
	// 	dataBase = gameDatabase;
	// }

// 	Game findGame(std::string name) {
// 		auto it = dataBase.find(name);
// 		if (it) {

// 		}
// 		return it;

// 		// Error
// 		throw std::runtime_error("Game not found");
// 	}

// 	void addGame (Game newGame, ts::Tree newConfig) {
// 		// Quetion: Check duplicates -> how to handle 
// 		dataBase.at(newGame.GetGameName())=newConfig;
// 	}

// 	void removeGame (std::string name) {
// 		// Quetion: if duplicates allowed -> how to handle 
// 		dataBase.erase(name);
// 		// Quetion: How to get game's configuration by game name
// 		// delete elements in defaultConfigurations and allGames

// 		// Error
// 		throw std::runtime_error("Game not found");
// 	}

// 	std::vector<ts::Tree> getDefaultConfigurations() {
// 		return defaultConfigurations;
// 	}
// }
 	