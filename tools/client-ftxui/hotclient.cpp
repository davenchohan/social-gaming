#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include "Client.h"
#include "ClientWrapper.h"
#include "ParserLibrary.h"
#include "Constants.h"
#include "GameComponent.h"

#include "LandingPage.h"
#include "CreateGamePage.h"
#include "NamePage.h"
#include "LoadingPage.h"
#include "JoinGamePage.h"
#include "MainPageContent.h"
#include "CreateGameSessionPage.h"
#include "TestGameComponent.h"

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border



using namespace ftxui;
using Json = nlohmann::json;







// FUNCTIONS #####################################################
// ###########################################################
// placeholder (until parser library is implemented)
void handleServerResponse(
                    const std::string& response, 
                    std::string& state, 
                    std::string& description, 
                    std::string& type, 
                    std::vector<std::string>& options, 
                    int& selection,
                    std::string& prompt, 
                    std::string& input, 
                    std::string& button, 
                    std::string& field, 
                    std::string& endpoint,
                    int& view_state) {
  // decompose server resonse
  RequestParser parser{response};

  Json jsonObj = Json::parse(response);
  state = jsonObj["state"].template get<std::string>();
  button = jsonObj["button"].template get<std::string>();
  description = jsonObj["description"].template get<std::string>();
  type = jsonObj["type"].template get<std::string>();
  prompt = jsonObj["prompt"].template get<std::string>();
  endpoint = jsonObj["endpoint"].template get<std::string>();
  field = jsonObj["field"].template get<std::string>();

  input = "";
  options.clear();
  selection = 0;

  if(type == "selection") {
    options = jsonObj["options"].template get<std::vector<std::string>>();
    view_state = 2;
  }
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

  

  // DATA ######################################################
  // data (varaibles) for interactive components to be
  // passed to pages 
  // ###########################################################

  // TEST VARIABLE FOR STORING JSON RESPONSE FROM BACKEND
  std::string test_json_response = "default";

  // main rendering data variables
  std::string state = "__uninitialized__";
  std::string description = "Welcome to Social Gaming Platform!";
  std::string type = "init";
  std::vector<std::string> options_;
  int selection_ = 0;
  std::string prompt = "__uninitialized__";
  std::string input = "__uninitialized__";
  std::string buttonLabel = "__uninitialized__";
  std::string field = "__uninitialized__";
  std::string endpoint = "__uninitialized__";



  // screen view state 0: landing page 1: game play
  int view_state = 0;

  //User name for the client
  std::string userName;
  

  // DATA - landing page
  std::vector<std::string> tab_values {
    "CREATE GAME SESSION",
    "JOIN GAME SESSION",
  };
  int tab_selected = 0;


// COMPONENTS ################################################
// page components
// * tab_view,
// * actionButton,
// * optionSelector,
// * inputComponent,
// ###########################################################

  // SUBPAGES/TABS
  int create_pagenum = 0;
  auto namePageElements = Pages::namePage(create_pagenum, view_state, client, userName);

  auto loadingPageElements = Pages::loadingPage(create_pagenum, view_state, client);

  // maybes allow for components to be shown conditionally  
  // the first argument is the component the second is the boolean
  // we will use this to render the different pages requrired for the desktop
  // by passing the components into this as a component and then having the renderer call render on page content 

  // PASSING LIST OF COMPONENTS
  // auto pageContent = Container::Vertical(game_page_components) | flex;

  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto actionButton = Button(&buttonLabel, [&] {
    if(type == "display") {
      client.send(endpoint + ",");
      return;
    }

    RequestConstructor reqConstructor(endpoint);
    if(type == "selection") {
      reqConstructor.appendItem(field, options_[selection_]);
    }else if(type == "input") {
      reqConstructor.appendItem(field, input);
    }

    auto json_string = reqConstructor.ConstructRequest();
    GetGameName getGameName = GetGameName(json_string);
    wrapper.sendReq(endpoint, getGameName, client);
  }) | Maybe([&] { return (endpoint != ""); });
  auto optionSelector = Radiobox(&options_, &selection_);
  auto inputComponent = Input(&input, "Enter here");
  auto tab1 = Renderer([&] {
    wrapper.sendNoBody(constants::ReqType::GETGAMES, client);
    return vbox({
      separator(),
      paragraph(" "),
    });
  });
  auto tab2 = Renderer([&] {
    type = "input";
    prompt = "Invite code:";
    input = "";
    buttonLabel = "Join";
    field = "InviteCode";
    endpoint = "ReqJoinGame";
    return vbox({
      separator(),
      paragraph(" "),
    });
  });

  auto tab_toggle = Toggle(&tab_values, &tab_selected);
  auto tab_container = Container::Tab({
    Container::Vertical({ tab1 }),
    Container::Vertical({ tab2 }),
  }, &tab_selected);


  auto tab_view = Container::Vertical({
    tab_toggle,
    tab_container,
  });
  auto main_container = Container::Vertical({
    tab_view,
    actionButton,
    optionSelector,
    inputComponent,
  });


  // MAIN RENDERER #############################################
  // renderer
  // handler
  // -------------
  // the actual rendering of the screen is done here 
  // the initial component that gets passed into the Renderer seems to be the only one that is interactive
  // multiple components can be grouped into containers so that multiple can be interactive 
  // ###########################################################

  // wrapper.sendNoBody(constants::ReqType::GETGAMES, client);

  auto renderer2 = Renderer(main_container, [&] {
    if(type == "display") {
      return vbox({
        vbox({
          tab_view->Render(),
          hbox({
            paragraph(description),
          }),
          hbox({
            actionButton->Render(),
          }),
        }) | flex | borderStyled(ROUNDED),
        vbox({
          paragraph(test_json_response) | color(Color::GreenLight),
        }),
      }) | flex;
    }else if(type == "selection") {
      return vbox({
        vbox({
          tab_view->Render(),
          hbox({
            paragraph(prompt),
          }),
          hbox({
            optionSelector->Render(),
          }),
          hbox({
            actionButton->Render(),
          }),
          hbox({
            paragraph(endpoint + ' ' + field),
          }),
        }) | flex | borderStyled(ROUNDED),
        vbox({
          paragraph(test_json_response) | color(Color::GreenLight),
        }),
      }) | flex;
    }else if(type == "input") {
      return vbox({
        vbox({
          tab_view->Render(),
          hbox({
            paragraph(prompt),
          }),
          hbox({
            inputComponent->Render(),
          }),
          hbox({
            actionButton->Render(),
          }),
          hbox({
            paragraph(endpoint + ' ' + field),
          }),
        }) | flex | borderStyled(ROUNDED),
        vbox({
          paragraph(test_json_response) | color(Color::GreenLight),
        }),
      }) | flex;
    }else if(type == "init") {
      return vbox({
        vbox({
          tab_view->Render(),
          hbox({
            paragraph(description),
          }),
        }) | flex | borderStyled(ROUNDED),
        vbox({
          paragraph(test_json_response) | color(Color::GreenLight),
        }),
      }) | flex;
      }else {
      return vbox({
          vbox({
            hbox({
              paragraph("Something went wrong. Please refresh your browser."),
            }),
          }) | flex | borderStyled(ROUNDED),
          vbox({
            paragraph(test_json_response) | color(Color::GreenLight),
          }),
        }) | flex;
    }
  });

  auto mainPage = Container::Vertical({
        Renderer([&] {
              return renderer2;
          }),
    });

  auto pageContent = Container::Vertical({
    mainPage | Maybe([&] {return view_state == 2;}),
    namePageElements | Maybe([&] {return view_state == 0;}),
    loadingPageElements | Maybe([&] {return view_state == 1;}),
  }) | flex;

  auto main_container2 = Container::Vertical({
    pageContent,
  });

  auto renderer = Renderer(main_container2, [&] {
    return vbox({
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

      handleServerResponse(response, state, description, type, options_, selection_, prompt, input, buttonLabel, field, endpoint, view_state); 
      screen.RequestAnimationFrame();
    }

    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_IN_MS));
  }

  return 0;
}