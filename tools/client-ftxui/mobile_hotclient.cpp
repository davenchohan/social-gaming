#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.h"
#include "MobileLandingPage.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;


// STYLE #####################################################
// styling can be defined outside of component definitions
// -----------------------------------------------------------
// This is a helper function to create a button with a custom style.
// The style is defined by a lambda function that takes an EntryState and
// returns an Element.
// We are using `center` to center the text inside the button, then `border` to
// add a border around the button, and finally `flex` to make the button fill
// the available space.
// ###########################################################


int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }

// DATA ######################################################
// data (varaibles) for interactive components to be
// passed to pages 
// ###########################################################
  // global variable
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


  // variables - join game session page
  int pagenum = 0;
  std::string invite_code;
  std::string display_name;

// COMPONENTS ################################################
// page components
// * landing page
//    * create game session page
//    * join game session page
// ###########################################################

  // SUBPAGES/TABS
   auto mobileLandingPageElements = Pages::MobileLandingPage(pagenum, invite_code, display_name, client);
  // auto createGameElements = Pages::CreateGame(showLanding, showJoin, showCreate, client);



//components can be grouped together so that they can be passed into the render together 


  // maybes allow for components to be shown conditionally  
  // the first argument is the component the second is the boolean
  // we will use this to render the different pages requrired for the desktop
  // by passing the components into this as a component and then having the renderer call render on page content 
  auto pageContent = Container::Vertical({
    mobileLandingPageElements,
  }) | flex;

  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto main_container = Container::Vertical({
    //homeButton,
    pageContent,
  });


  // MAIN RENDERER #############################################
  // renderer
  // handler
  // -------------
  // the actual rendering of the screen is done here 
  // the initial component that gets passed into the Renderer seems to be the only one that is interactive
  // multiple components can be grouped into containers so that multiple can be interactive 
  // ###########################################################
  auto renderer = Renderer(main_container, [&] {
    return vbox({
      hbox({
        text("Hot Root Soup mobile"),
      }),
      // filler(),
      hbox({
        // filler(),
        pageContent->Render(),
        // filler(),
      }) | flex | borderStyled(ROUNDED),
       filler(),
    }) |flex ;
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