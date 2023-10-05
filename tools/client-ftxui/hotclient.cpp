#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.h"
#include "LandingPage.h"
#include "CreateGamePage.h"
#include "JoinGamePage.h"
#include "CreateGameSessionPage.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

// This is a helper function to create a button with a custom style.
// The style is defined by a lambda function that takes an EntryState and
// returns an Element.
// We are using `center` to center the text inside the button, then `border` to
// add a border around the button, and finally `flex` to make the button fill
// the available space.

//styling can be defined outside of component definitions
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

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }



  networking::Client client{argv[1], argv[2]};

  bool done = false;
  
  std::string entry;
  std::vector<Element> history;


  // leftover from web-chat but might be useful 
  Component entryField = Input(&entry, "Enter server code here.");
  auto onTextEntry = [&done, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else {
      client.send(std::move(text));
    }
  };

    

  //booleans for triggering different pages 
  bool showJoin = false;
  bool showCreate = false;
  bool showLanding = true;

  // variables for landing page
  std::vector<std::string> tab_values {
    "CREATE GAME SESSION",
    "TEXT",
    "JOIN GAME SESSION",
  };
  int tab_selected = 0;

  //  game session data
  std::vector<std::string> radiobox_list = {
      "Game 1",
      "Game 2",
      "Game 3",
      "Game 4",
  };
  int radiobox_selected = 0;



  // content inside tabs
  auto createGameSessionPage = Pages::CreateGameSession(radiobox_list, radiobox_selected, client);

  //this is how pages will be passed back to the main page 
  auto landingPageElements = Pages::Landing(createGameSessionPage, showLanding, showJoin, showCreate, client, tab_values, tab_selected, entry);
  auto joinGameElements = Pages::JoinGame(showLanding, showJoin, showCreate, client);
  auto createGameElements = Pages::CreateGame(showLanding, showJoin, showCreate, client);

  


//components can be grouped together so that they can be passed into the render together 
 auto homeButton = Container::Vertical({
    Container::Horizontal({
      Button(
        "Home", [&] { 
          showJoin = false;
          showCreate = false;
          showLanding = true;
          client.send(std::move("home"));
        }, ButtonStyle()
      ), 
    }) | flex,
  });


  // maybes allow for components to be shown conditionally  
  // the first argument is the component the second is the boolean
  // we will use this to render the different pages requrired for the desktop
  // by passing the components into this as a component and then having the renderer call render on page content 
  auto pageContent = Container::Vertical({
    landingPageElements,
    // Container::Horizontal({
    //   Maybe(landingPageElements, &showLanding),
    //   Maybe(joinGameElements, &showJoin),
    //   Maybe(createGameElements, &showCreate),
    //   }),
      // }) | size(HEIGHT, EQUAL, 10),
      
  }) | flex;



  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto main_container = Container::Vertical({
    homeButton,
    pageContent,
  });

  //the actual rendering of the screen is done here 
  //the initial component that gets passed into the Renderer seems to be the only one that is interactive
  //multiple components can be grouped into containers so that multiple can be interactive 

  //this is the main renderer 

  auto renderer = Renderer(main_container, [&] {
    return vbox({
      hbox({homeButton->Render(),filler(),text("Hot Root Soup")}),
        // filler(),
        hbox({
          // filler(),
          pageContent->Render(),
          // filler(),
        }) | flex | borderStyled(ROUNDED),
        // filler(),
    }) |
    flex;
    // flex | border | color(Color::GreenLight);
  });


  auto screen = ScreenInteractive::Fullscreen();



  // event handler that is passed to the loop function to catch events occuring 
  auto handler = CatchEvent(renderer, [&entry,&onTextEntry](const Event& event) {
    if (event == Event::Return) {
      onTextEntry(std::move(entry));
      entry.clear();
      return true;
    }
    return false;
  });



  const int UPDATE_INTERVAL_IN_MS = 50;

  //this loop runs consitantly at the set time interval
  Loop loop(&screen, handler);
  while (!done && !client.isDisconnected() && !loop.HasQuitted()) {
    try {
      client.update();
    } catch (std::exception& e) {
      history.push_back(text("Exception from Client update:"));
      history.push_back(text(e.what()));
      done = true;
    }

    auto response = client.receive();
    if (!response.empty()) {
      history.push_back(paragraphAlignLeft(response));
      screen.RequestAnimationFrame();
    }

    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_IN_MS));
  }

  return 0;
}