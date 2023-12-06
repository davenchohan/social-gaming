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
    Component MainPageContent(
                        std::string& state, 
                        std::string& description, 
                        std::string& type, 
                        std::vector<std::string>& options, 
                        int& selection, 
                        std::string& prompt, 
                        std::string& input, 
                        std::string& buttonLabel, 
                        std::string& endpoint,
                        networking::Client& client);
}
