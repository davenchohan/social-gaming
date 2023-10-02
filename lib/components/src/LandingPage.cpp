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
#include "LandingPage.h"

using namespace ftxui;

namespace Pages{
ButtonOption ButtonStyle() {
  auto option = ButtonOption::Animated();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) {
      element |= bold;
    }
    return element | center | borderEmpty | flex;
  };
  return option;
}


Component Landing(bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client){
    auto buttonSection = Container::Vertical({
    Container::Horizontal({
      Button(
        "Create new game", [&] { 
          showJoin = false;
          showCreate = true;
          showLanding = false;
          client.send(std::move("createGame"));
        }, ButtonStyle()
      ),
      Button(
        "Join Game", [&] { 
          showJoin = true;
          showCreate = false;
          showLanding = false;
          client.send(std::move("getActiveGames"));
          
        }, ButtonStyle()
      ),   
    }) | flex,
  });
  auto landingWindow = Window({.inner = buttonSection, .title = "landing page",  .width = 80, .height = 60,});
  return landingWindow;

 
}
}


