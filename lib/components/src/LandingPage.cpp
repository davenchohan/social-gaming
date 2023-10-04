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


Component Landing(bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client, std::vector<std::string> &tab_values, int &tab_selected, std::string &entry){
  // tab view
    auto tab_toggle = Toggle(&tab_values, &tab_selected);
    auto tab_container = Container::Tab({
      Button("A Button that clears the input in tab3", [&]{
        entry.clear();
      }),
      Renderer([] {
        return text("A Text");
      }),
      Container::Vertical({
        Renderer([] {
          return text("Enter an invite code to join a game!");
        }),
        Input(&entry, "invite code:"),
      }),
    }, &tab_selected);

    auto tab_view = Container::Vertical({
      tab_toggle,
      tab_container,
    });

  auto landingWindow = Window({.inner = tab_view, .title = "landing page",  .width = 80, .height = 60,});
  return tab_view;
  // return landingWindow;
}
}


