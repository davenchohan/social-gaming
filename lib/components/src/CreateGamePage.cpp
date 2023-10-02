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
#include "CreateGamePage.h"

using namespace ftxui;
namespace Pages{
Component CreateGame(bool &showLanding, bool &showGame, bool &showCreate, networking::Client &client){
     auto page = Window({.title = "Create Game"});
     return page;
}
}