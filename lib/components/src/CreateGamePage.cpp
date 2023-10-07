#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.h"
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp"
#include "CreateGamePage.h"

using namespace ftxui;
namespace Pages
{

std::vector<std::string> games = {
  "Rock, Paper, Scissors",
  "another game",
  "some other game",
  "yet another game",
};
//took this from Landing page just to get it working.
//TODO:Make this correct for this page
ButtonOption ButtonStyle2() {
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
Component CreateGame(bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client){
    

    int selected = 0;
    auto dropDownSection = Container::Vertical({
      Container::Horizontal({
        Dropdown(&games, &selected),
      }),
        Container::Horizontal({Button(
          "Cancel", [&] {
            showJoin = false;
            showCreate = false;
            showLanding = true; 
            client.send(std::move("Cancelling Game Creation"));
          }, ButtonStyle2()
        ),
        Button(
          "Proceed", [&] {
            showJoin = false;
            showCreate = false;
            showLanding = true; 
            client.send(std::move("Game Selected, moving to options page"));
          }, ButtonStyle2()
        ),
      }),
    });
    auto createWindow = Window({.inner = dropDownSection, .title = "Create Game", .width = 80, .height = 60});
    return createWindow;


}
}