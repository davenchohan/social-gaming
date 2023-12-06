#pragma once

#include <map>
#include "Game.h"
// #include <cpp-tree-sitter.h>
// #include "../../../tools/parser/include/parser_test.h"

// extern "C" {
// TSLanguage *tree_sitter_socialgaming();
// }


// Games Database stores the map <game_name, game_JSON_configuration>
// No duplicates are cosidered
// class GamesDatabase
// {
// public:
// 	// Constructors
// 	GamesDatabase(std::vector<std::string> games_paths);
// 	// Destructors
// 	~GamesDatabase();
	
// 	Game findGame(std::string game_name);
// 	void addGame (SGParser::SGParser gameParser); 
// 	void removeGame (std::string game_name); 
// 	json getDefaultConfigurations(std::string name); 


// private:
// 	std::map<std::string, json> dataBase;	
// };