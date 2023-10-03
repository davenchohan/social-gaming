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
// duplicate button styling definition in landing page: might be better to put in one location for better design
ButtonOption NavButtonStyle() { 
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


Component JoinGame(bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client, std::string &entry){
// Component JoinGame(bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client){

    Component entryField = Input(&entry, "Enter invite code here.");
    auto joinGameView = Container::Vertical({
        entryField,
        Container::Horizontal({
            Button(
                "Cancel", [&] { 
                showJoin = false;
                showCreate = false;
                showLanding = true;
                entry.clear();
                client.send(std::move("home"));
                }, NavButtonStyle()
            ),
            Button(
                "Join (redirects to Home at the moment)", [&] { 
                showJoin = false;
                showCreate = false;
                showLanding = true;
                client.send(std::move("home"));
                }, NavButtonStyle()
            ),   
        }) | flex,
    });



/*
   std::vector<std::string> tab_values{
      "tab_1",
      "tab_2",
      "tab_3",
  };
  int tab_selected = 1;
  auto tab_toggle = Toggle(&tab_values, &tab_selected);
 
  std::vector<std::string> tab_1_entries{
      "Forest",
      "Water",
      "I don't know",
  };
  int tab_1_selected = 0;
 
  std::vector<std::string> tab_2_entries{
      "Hello",
      "Hi",
      "Hay",
  };
  int tab_2_selected = 0;
 
  std::vector<std::string> tab_3_entries{
      "Table",
      "Nothing",
      "Is",
      "Empty",
  };
  int tab_3_selected = 0;
  auto tab_container = Container::Tab(
      {
        // Input(&entry, "1: Enter invite code here."),
        // Input(&entry, "2: Enter invite code here."),
        // Input(&entry, "3: Enter invite code here."),
          Radiobox(&tab_1_entries, &tab_1_selected),
          Radiobox(&tab_2_entries, &tab_2_selected),
          Radiobox(&tab_3_entries, &tab_3_selected),
      },
      &tab_selected);
 
  auto tabView = Container::Vertical({
      tab_toggle,
      tab_container,
  });

  */

    auto page = Window({.inner = joinGameView, .title = "Join Game",.width = 80, .height = 60,});
    return page;
}
}