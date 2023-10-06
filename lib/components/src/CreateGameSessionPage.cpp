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
#include "CreateGameSessionPage.h"

using namespace ftxui;
namespace Pages{
Component CreateGameSession(int &create_pagenum, std::string &session_name, std::vector<std::string> &radiobox_list, int &radiobox_selected, networking::Client &client){

     const int max_pagenum = 1;// starting 0
     auto game_selector = Radiobox(&radiobox_list, &radiobox_selected);

     auto title1 = Renderer([] {
          return text("Select a game") | bold;
     });
     auto title2 = Renderer([] {
          return text("Basic Configuration") | bold | color(Color::GreenLight);
     });
     auto title3 = Renderer([] {
          return paragraph("Game Session Name");
     });

     auto selectedGameTitle = Renderer([&] {
          return paragraph("Selected game: " + radiobox_list[radiobox_selected]);
     });

     // pages
     auto page1 = Container::Vertical({
          title1,
          game_selector,
     });
     auto page2 = Container::Vertical({
          title2,
          selectedGameTitle,
          title3,
          Input(&session_name, "Enter game session name."),
     });

     auto page = Container::Vertical({
          page1 | Maybe([&] {return create_pagenum == 0;}),
          page2 | Maybe([&] {return create_pagenum == 1;}),
          Container::Horizontal({
            Button("Back", [&]{
               create_pagenum--;
            }) | Maybe([&] {return create_pagenum > 0;}),
            Button("Next", [&]{
               create_pagenum++;
            }) | Maybe([&] {return create_pagenum < max_pagenum;}),
            Button("Create", [&]{}) | Maybe([&] {return create_pagenum == max_pagenum;}),// transfer page to game play page
        }) | flex,
     });

     // auto page = Window({.inner = game_selector, .title = "Create Game session",.width = 80, .height = 60});
     // return page;
     return page;
}
}