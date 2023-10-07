#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" 
#include "CreateNewGamePage.h"

std::string name;
std::string player_range;
std::string audience;

Component input_name = Input(&name, "Game Name");
Component input_player_range = Input(&player_range, "Range of Players: input as (minimum players, maximum players)");
Component input_audience = Input(&audience, "Audience: input as (true or false)");

using namespace ftxui;
namespace Pages{
Component CreateNewGame(bool &create_new_game, networking::Client &client){
     auto page = Container::Vertical({input_name, input_player_range, input_audience});
     return page;
}
}