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
#include "ClientWrapper.h"
#include "Constants.h"

using namespace ftxui;



// FUNCTIONS #####################################################
// ###########################################################
// placeholder (until parser library is implemented)
std::vector<std::string> parseServerResponseGameList(const std::string &response) {

    // find the start of json
    size_t start = response.find('[');
    size_t end = response.find(']');
    if(start != std::string::npos && end != std::string::npos) {
        if(start <= end) {
            std::vector<std::string> result_list;
            std::string_view response_view{response};
            std::string_view list_view = response_view.substr(start+1, end - start-1);
            std::cout << list_view << std::endl;

            // find pair of '
            bool start = false;
            std::string_view games = list_view;
            size_t position = games.find("'");

            while(position != std::string_view::npos) {
                if(start) {
                    // found the end
                    std::string game{games.substr(0, position)};
                    std::cout << "found: " << game << std::endl;
                    result_list.push_back(game);
                    start = false;
                }else {
                    // found teh start
                    start = true;
                }
                games = games.substr(position+1, games.size()-position);
                std::cout << "[" << games << "]" << std::endl;
                position = games.find("'");
            }

            return result_list;
        }
    }

    return {};
}


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



// DATA ######################################################
// data (varaibles) for interactive components to be
// passed to pages 
// ###########################################################
  // TEST VARIABLE FOR STORING JSON RESPONSE FROM BACKEND
  std::string test_json_response = "default";

  // global variable
  networking::Client client{argv[1], argv[2]};
  networking::ClientWrapper wrapper;
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
  
  wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);

  // DATA - landing page
  std::vector<std::string> tab_values {
    "CREATE GAME SESSION",
    "JOIN GAME SESSION",
  };
  int tab_selected = 0;

  // data - create game session page
  std::vector<std::string> radiobox_list = {
      "Class Quiz",
      "Rock, Paper, Scissors",
      "Tic Tac Toe",
      "Tetris",
  };
  int radiobox_selected = 0;

  // variables - join game session page
  int pagenum = 0;
  std::string invite_code;
  std::string display_name;
  // variabels - create game session page
  int create_pagenum = 0;
  std::string game_session_name;


// COMPONENTS ################################################
// page components
// * landing page
//    * create game session page
//    * join game session page
// ###########################################################

  // SUBPAGES/TABS
  auto createGameSessionElements = Pages::CreateGameSession(create_pagenum, game_session_name, radiobox_list, radiobox_selected, client);
  auto joinGameSessionElements = Pages::JoinGame(pagenum, invite_code, display_name, client);

  // MAIN PAGE 
  auto landingPageElements = Pages::Landing(createGameSessionElements, joinGameSessionElements, client, tab_values, tab_selected, entry);

  // auto createGameElements = Pages::CreateGame(showLanding, showJoin, showCreate, client);



//components can be grouped together so that they can be passed into the render together 
 auto homeButton = Container::Vertical({
    Container::Horizontal({
      Button(
        "Home", [&] { 
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
  }) | flex;

  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto main_container = Container::Vertical({
    homeButton,
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
        homeButton->Render(),
        filler(),
        text("Hot Root Soup"),
      }),
      // filler(),
      hbox({
        // filler(),
        pageContent->Render(),
        // filler(),
      }) | flex | borderStyled(ROUNDED),
      // filler(),
      hbox({
        // history.back(),
        paragraph(test_json_response) | color(Color::GreenLight),
      }),
      // history.back() | flex,
    }) | flex;
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
      test_json_response = response;
      radiobox_list = parseServerResponseGameList(test_json_response);
      screen.RequestAnimationFrame();
    }

    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_IN_MS));
  }

  return 0;
}