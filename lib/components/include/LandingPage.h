#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../../networking/include/Client.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" 



using namespace ftxui;
namespace Pages{
    Component Landing(Component createGameSession, Component joinGameSession, bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client, std::vector<std::string> &tab_values, int &tab_selected, std::string &entry);
}

