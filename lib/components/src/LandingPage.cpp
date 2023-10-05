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

// #include "CreateGameSessionPage.h"

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


Component Landing(Component createGameSession, bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client, std::vector<std::string> &tab_values, int &tab_selected, std::string &entry){
  auto sometext = Renderer([] {
        return paragraph("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
    });

  // tab view
    auto tab_toggle = Toggle(&tab_values, &tab_selected);
    auto tab_container = Container::Tab({
      createGameSession,
      // Button("A Button that clears the input in tab3", [&]{
      //   entry.clear();
      // }),
      Container::Vertical({
        sometext,
        Button("A Button that clears the input in tab3", [&]{
          entry.clear();
        }),
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



    // auto document = vbox({
    //     tab_toggle->Render(),
    //     separator(),
    //     tab_container->Render(),
    // }) | border;

    // auto renderer = Renderer(tab_view, [&] {
    // return vbox({
    //       tab_toggle->Render(),
    //       separator(),
    //       tab_container->Render(),
    //   }), | border;
    // });

    auto demo_container = Container::Vertical({
      Button(
        "demo button", [&] {}, ButtonStyle()
      ),
    });

  // auto landingWindow = Window({.inner = demo_container, .title = "landing page",  .width = 200, .height = 150,});
  // return document;
  // return landingWindow;
  return tab_view;
}
}


