#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include "Client.h"
#include "ClientWrapper.h"
#include "ParserLibrary.h"
#include "Constants.h"
#include "GameComponent.h"
#include "RandomIdGenerator.h"

#include "LandingPage.h"
#include "CreateGamePage.h"
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



class ConfigData {
  public:
  ConfigData(): username("") {}
  void setUsername(std::string& usrnm) {
    username = usrnm;
  }
  std::string& getUsername() {
    return username;
  }
  private:
  std::string username;
};

class DisplayData {
  public:
  DisplayData() = delete;
  DisplayData(
      std::string init_state,
      std::string init_description,
      std::string init_type,
      std::vector<std::string> init_options,
      std::string init_prompt,
      std::string init_button,
      std::string init_field,
      std::string init_endpoint) : 
      state(init_state), 
      description(init_description), 
      type(init_type),
      options(init_options),
      prompt(init_prompt),
      input(""),
      button(init_button),
      field(init_field),
      endpoint(init_endpoint) {}
  void updateDisplayData(const std::string& response) {
    Json jsonRes = Json::parse(response);
    state       = jsonRes["state"].template get<std::string>();
    button      = jsonRes["button"].template get<std::string>();
    description = jsonRes["description"].template get<std::string>();
    type        = jsonRes["type"].template get<std::string>();
    prompt      = jsonRes["prompt"].template get<std::string>();
    endpoint    = jsonRes["endpoint"].template get<std::string>();
    field       = jsonRes["field"].template get<std::string>();

    input = "";
    options.clear();
    selection = 0;

    if(type == "selection") {
      options = jsonRes["options"].template get<std::vector<std::string>>();
    }
  }

  std::string state;
  std::string description;
  std::string type;
  std::vector<std::string> options;
  int selection = 0;
  std::string prompt;
  std::string input;
  std::string button;
  std::string field;
  std::string endpoint;
};

class ViewManager {
  public:
  ViewManager() = default;
  ViewManager(ConfigData&& config, DisplayData&& disp, std::string_view address, std::string_view port): config_data(config), disp_data(disp), client(address, port) {}
  void handleServerResponse(const std::string& response) {
    disp_data.updateDisplayData(response);
  }
  void sendRequest(RequestConstructor reqConstructor) {
    if(disp_data.state == "init") {
      initRequest(reqConstructor);
      return;
    }
    
    if(disp_data.type == "display") {
      client.send(disp_data.endpoint + ",");
    }else if(disp_data.type == "selection") {
      reqConstructor.appendItem(disp_data.field, disp_data.options[disp_data.selection]);
    }else if(disp_data.type == "input") {
      reqConstructor.appendItem(disp_data.field, disp_data.input);
    }

    auto json_string = reqConstructor.ConstructRequest();
    GetGameName getGameName = GetGameName(json_string);
    wrapper.sendReq(disp_data.endpoint, getGameName, client);
  }
  void initRequest(RequestConstructor reqConstructor) {
    if (disp_data.input.empty()) {
      int randomID = RandomIdGenerator::generateUniqueId();
      disp_data.input = "Anon_Player";
      disp_data.input.append(std::to_string(randomID));
    }

    reqConstructor.appendItem(disp_data.field, disp_data.input);
    auto json_string = reqConstructor.ConstructRequest();
    GetGamesList getGamesList = GetGamesList(json_string);
    wrapper.sendReq(disp_data.endpoint, getGamesList, client);

    config_data.setUsername(disp_data.input);
    disp_data.input = "";
  }

  void setJoinTabData() {
    disp_data.state = "success";
    disp_data.type = "input";
    disp_data.input = "";
    disp_data.prompt = "Invite code: ";
    disp_data.button = "Join";
    disp_data.field = "InviteCode";
    disp_data.endpoint = "ReqJoinGame";
  }

  ConfigData config_data;
  DisplayData disp_data;
  networking::Client client;
  networking::ClientWrapper wrapper;
};


// STYLE #####################################################
// -----------------------------------------------------------
// * Button
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


  ConfigData config{};
  DisplayData disp{"init", "", "", {}, "Username: ", "Confirm", "misc", "ReqGetGamesList"};
  ViewManager viewMan{std::move(config), std::move(disp), argv[1], argv[2]};


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

  std::string test_json_response = "none";

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

  // all components that need to be interactive will be added to the main container.
  // this allows them to be tracked by the renderer
  // the component passed into here will need to be called with -> Render() again in the actual renderer 
  auto actionButton = Button(&viewMan.disp_data.button, [&] {
    RequestConstructor reqConstructor(viewMan.disp_data.endpoint);
    viewMan.sendRequest(reqConstructor);
  }) | Maybe([&] { return (viewMan.disp_data.endpoint != ""); });
  
  auto optionSelector = Radiobox(&viewMan.disp_data.options, &viewMan.disp_data.selection);
  auto inputComponent = Input(&viewMan.disp_data.input, "enter here");
  auto tab1 = Renderer([&] {
    viewMan.disp_data.endpoint = "ReqGetGamesList";
    RequestConstructor reqConstructor(viewMan.disp_data.endpoint);
    viewMan.sendRequest(reqConstructor);
    return vbox({
      separator(),
      paragraph(" "),
    });
  });
  auto tab2 = Renderer([&] {
    viewMan.setJoinTabData();
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

  auto display_view = [&]() {
    return vbox({
        vbox({
          tab_view->Render(),
          hbox({
            paragraph(viewMan.disp_data.description),
          }),
          hbox({
            actionButton->Render(),
          }),
        }) | flex | borderStyled(ROUNDED),
        vbox({
          paragraph(test_json_response) | color(Color::GreenLight),
        }),
      }) | flex;
  };
  auto selection_view = [&]() {
    return vbox({
      vbox({
        tab_view->Render(),
        hbox({
          paragraph(viewMan.disp_data.prompt),
        }),
        hbox({
          optionSelector->Render(),
        }),
        hbox({
          actionButton->Render(),
        }),
        hbox({
          paragraph(viewMan.disp_data.endpoint + ' ' + viewMan.disp_data.field),
        }),
      }) | flex | borderStyled(ROUNDED),
      vbox({
        paragraph(test_json_response) | color(Color::GreenLight),
      }),
    }) | flex;
  };
  auto input_view = [&]() {
    return vbox({
      vbox({
        tab_view->Render(),
        hbox({
          paragraph(viewMan.disp_data.prompt),
        }),
        hbox({
          inputComponent->Render(),
        }),
        hbox({
          actionButton->Render(),
        }),
        hbox({
          paragraph(viewMan.disp_data.endpoint + ' ' + viewMan.disp_data.field),
        }),
      }) | flex | borderStyled(ROUNDED),
      vbox({
        paragraph(test_json_response) | color(Color::GreenLight),
      }),
    }) | flex;
  };
  auto init_view = [&]() {
    return vbox({
      vbox({
        vbox({
            paragraphAlignCenter("Welcome to the Social Gaming Server developed by HotrootSoup!") | color(Color::GreenLight),
            paragraphAlignCenter("Please enter a username to start (^_^)") | color(Color::GreenLight),
            paragraph(" "),
            separator(),
        }),
        vbox({
          paragraph(" "),
          hbox({
            filler(),
            text(viewMan.disp_data.prompt),
            inputComponent->Render(),
            filler(),
          }),
          paragraph(" "),
        }),
        vbox({
          hbox({
            filler(),
            actionButton->Render(),
          }),
        }),
      }) | flex,
      vbox({ // debug
        separator(),
        paragraph("Next Request Endpoint: " + viewMan.disp_data.endpoint + ' ' + viewMan.disp_data.field) | color(Color::Cyan),
        paragraph("Server Response: " + test_json_response) | color(Color::GreenLight),
      }),
    }) | flex;
  };
  auto error_view = [&]() {
    return vbox({
      vbox({
        hbox({
          paragraph("Something went wrong. Please reload your browser."),
        }),
      }) | flex | borderStyled(ROUNDED),
      vbox({
        paragraph(test_json_response) | color(Color::GreenLight),
      }),
    }) | flex;
  };

  auto renderer = Renderer(main_container, [&] {
    if(viewMan.disp_data.state == "init") {
      return init_view();
    }

    if(viewMan.disp_data.state == "success") {
      if     (viewMan.disp_data.type == "display")   { return display_view(); }
      else if(viewMan.disp_data.type == "selection") { return selection_view(); }
      else if(viewMan.disp_data.type == "input")     { return input_view(); }
    }else {
      return error_view();
    }
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

      viewMan.handleServerResponse(response);
      // handleServerResponse(response, state, description, type, options_, selection_, prompt, input, button, field, endpoint); 
      screen.RequestAnimationFrame();
    }

    loop.RunOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_IN_MS));
  }

  return 0;
}