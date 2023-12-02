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
#include "ParserLibrary.h"

using namespace ftxui;
namespace Pages{
Component namePage(int &pagenum, int &view_state, networking::Client &client, std::string &userName) {

     const int max_pagenum = 0;// starting 0
     //auto game_selector = Radiobox(&radiobox_list, &radiobox_selected);


     auto title1 = Renderer([] {
          return paragraph("Enter your Username");
     });

     // pages
     auto page0 = Container::Vertical({
          title1,
          Input(&userName, "Type your username here."),
          Renderer([] {
               return filler();
            }),
          Container::Horizontal({
            Renderer([] {
               return filler();
            }),
            Button("Confirm", [&]{
               // TODO: send name to server
               // std::string req_game_config_list = radiobox_list[radiobox_selected];
               // wrapper.sendReq(constants::ReqType::, )

               view_state = 1;
            }),
        }) | flex,
     });

     auto page = Container::Vertical({
          page0 | Maybe([&] {return pagenum == 0;}),
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