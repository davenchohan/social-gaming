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

Component CreateGame( bool &showGameOptions, bool &showGameSelecion, networking::Client &client){
     auto selectionScreen = Container::Vertical({
  
      Button(
        "game 1", [&] {
          showGameOptions = true;
          showGameSelecion = false;
          client.send(std::move("createGame"));
        }, ButtonStyle2()
      ),
        Button(
        "game 2", [&] {
          client.send(std::move("createGame"));
        }, ButtonStyle2()
      ),
        Button(
        "game 3", [&] {
          client.send(std::move("createGame"));
        }, ButtonStyle2()
      ),
        Button(
        "game 4", [&] {
          client.send(std::move("createGame"));
        }, ButtonStyle2()
      ),
       
    }) | flex;

     auto optionsScreen = Container::Vertical({
  
      Button(
        "this would be the options screen \n  input game parameters \n input questions \n ........", [&] {
          showGameOptions = false;
          showGameSelecion = true;
          client.send(std::move("createGame"));
        }, ButtonStyle2()
      ),
        
       
    }) | flex;

    auto pageContent = Container::Vertical({
     Maybe(selectionScreen,&showGameSelecion),
     Maybe(optionsScreen,&showGameOptions),
    });

    return pageContent;
}
}