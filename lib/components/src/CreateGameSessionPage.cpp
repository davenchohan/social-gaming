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

#include "ClientWrapper.h"
#include "Constants.h"

using namespace ftxui;
namespace Pages{
Component CreateGameSession(int &create_pagenum, std::string &session_name, std::vector<std::string> &radiobox_list, int &radiobox_selected, int &view_state, networking::Client &client){

     networking::ClientWrapper wrapper;
     wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);

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
          Renderer([] {
               return filler();
            }),
          Container::Horizontal({
            Renderer([] {
               return filler();
            }),
            Button("Next", [&]{
               // TODO: get list of fields to be configured
               // std::string req_game_config_list = radiobox_list[radiobox_selected];
               // wrapper.sendReq(constants::ReqType::, )

               // testing sending request from component
               GetGame getGame = GetGame("252434");
               wrapper.sendReq(constants::ReqType::DEMOGETGAME, getGame, client);


               create_pagenum++;

            }),
        }) | flex,
     });
     auto page2 = Container::Vertical({
          title2,
          selectedGameTitle,
          title3,
          Input(&session_name, "Enter game session name."),
          //gameConfig,
          Renderer([] {
               return filler();
            }),
          Container::Horizontal({
            Button("Back", [&]{
               wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);
               create_pagenum--;
            }) | Maybe([&] {return create_pagenum > 0;}),
            Renderer([] {
               return filler();
            }),
            Button("Create", [&]{
               view_state = 1;
            }),// transfer page to game play page
        }) | flex,
     });

     auto page = Container::Vertical({
          page1 | Maybe([&] {return create_pagenum == 0;}),
          page2 | Maybe([&] {return create_pagenum == 1;}),
     //      Renderer([] {
     //           return filler();
     //        }),
     //      Container::Horizontal({
     //        Button("Back", [&]{
     //           create_pagenum--;
     //        }) | Maybe([&] {return create_pagenum > 0;}),
     //        Renderer([] {
     //           return filler();
     //        }),
     //        Button("Next", [&]{
     //           create_pagenum++;
     //        }) | Maybe([&] {return create_pagenum < max_pagenum;}),
     //        Button("Create", [&]{}) | Maybe([&] {return create_pagenum == max_pagenum;}),// transfer page to game play page
     //    }) | flex,
     });

     // auto page = Window({.inner = game_selector, .title = "Create Game session",.width = 80, .height = 60});
     return page;
}
}