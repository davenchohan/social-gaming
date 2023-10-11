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
   Component MobileLandingPage(int &pagenum, std::string &code, std::string &name, networking::Client &client);
}

