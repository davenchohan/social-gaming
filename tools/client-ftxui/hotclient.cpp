#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.h"
#include "LandingPage.h"
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



  Component entryField = Input(&entry, "Enter server code here.");
  auto onTextEntry = [&done, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else {
      client.send(std::move(text));
    }
  };

    


  bool gameList = false;
  bool createNew = false;
  bool showLanding = true;

  //this is how pages will be passed back to the main page 
  auto landingPageElements = Pages::Landing(showLanding, createNew, gameList, client);

  auto gameBrowser = Renderer([]{ return text("this will be a game browser"); });
  auto newGame = Renderer([]{ return text("this will be a new game screen"); });
  
//components can be grouped together so that they can be passed into the render together 
 auto buttonSection = Container::Vertical({
    Container::Horizontal({
      Button(
        "Create new game", [&] { 
          gameList = false;
          createNew = true;
          showLanding = false;
          client.send(std::move("createGame"));
        }, ButtonStyle()
      ),
      Button(
        "Start new existing Game", [&] { 
          gameList = true;
          createNew = false;
          showLanding = false;
          client.send(std::move("getGamesList"));
          
        }, ButtonStyle()
      ),
      Button(
        "Join Game", [&] { 
          gameList = false;
          createNew = false;
          showLanding = true;
          client.send(std::move("getActiveGames"));
          
        }, ButtonStyle()
      ),   
    }) | flex,
  });


  // maybes allow for components to be shown conditionally  
  // the first argument is the component the second is the boolean
  // we will use this to render the different pages requrired for the desktop
  // by passing the components into this as a component and then ha 
  auto pageContent = Container::Vertical({
    Container::Horizontal({
      Maybe(landingPageElements, &showLanding),
      Maybe(gameBrowser, &gameList),
      Maybe(newGame, &createNew),
      }) | size(HEIGHT, EQUAL, 5),
      
    });
  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto main_container = Container::Vertical({
      buttonSection,
      pageContent,
  });

  //the actual rendering of the screen is done here 
  //the initial component that gets passed into the Renderer seems to be the only one that is interactive
  //multiple components can be grouped into containers so that multiple can be interactive 

  //this is the main renderer 

  auto renderer = Renderer(main_container, [&] {
    return vbox({
      text("Hot Root Soup"),
      buttonSection->Render(),
      pageContent->Render(),
      window(text("Server responses "),yframe(
        vbox(history)
      ))| yflex,
      
    }) |
    flex | border | color(Color::GreenLight);
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