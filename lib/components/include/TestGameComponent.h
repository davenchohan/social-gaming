#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include "../../networking/include/Client.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" 
#include "Constants.h"
#include "GameComponent.h" // struct ComponentData

using namespace ftxui;
namespace Pages{
Component TestGamePage(std::vector<std::vector<std::string>> &values, std::vector<int> &selected_items, std::vector<Element> &text_list,
    Elements &texts, std::vector<std::string> &options, int &selection, networking::Client &client); 
}