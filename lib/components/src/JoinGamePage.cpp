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
#include "JoinGamePage.h"

using namespace ftxui;

namespace Pages{
Component JoinGame(bool &showLanding, bool &showJoin, bool &showCreate,  std::vector<std::string> &tab_values, int &tab_selected, networking::Client &client){
     
  auto tab_toggle = Toggle(&tab_values, &tab_selected);
 
  auto tab_container = Container::Tab(
      {
        Button("test",[&]{}),
        Button("test2",[&]{}),
        Button("test3",[&]{}),
      },
      &tab_selected);
   std::string entry;
  auto Tabs_container = Input(&entry, "Enter server code here.");
  auto onTextEntry = [&showCreate, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      showCreate = true;
    } else {
      client.send(std::move(text));
    }
  };
    auto page = Window({.inner = Tabs_container, .title = "Join Game",.width = 80, .height = 60,});
    return Tabs_container;
}
}