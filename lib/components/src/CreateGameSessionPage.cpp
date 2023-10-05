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
#include "CreateGameSessionPage.h"

using namespace ftxui;
namespace Pages{
Component CreateGameSession(std::vector<std::string> &radiobox_list, int &radiobox_selected, networking::Client &client){
     
     auto game_selector = Radiobox(&radiobox_list, &radiobox_selected);
     // auto page = Window({.inner = game_selector, .title = "Create Game session",.width = 80, .height = 60});
     // return page;
     return game_selector;
}
}