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
#include "RandomIdGenerator.h"

using namespace ftxui;
namespace Pages{
Component namePage(int &pagenum, int &view_state, networking::Client &client, std::string &userName) {

     //const int max_pagenum = 0;// starting 0
     //auto game_selector = Radiobox(&radiobox_list, &radiobox_selected);

     bool errorName = false;
     
     networking::ClientWrapper wrapper;

     auto title1 = Renderer([] {
          return paragraph("Enter your Username");
     });

    auto errorMessage = Renderer([] {
          return paragraph("Please enter a valid name!");
     });
    auto maybe_component = Maybe(errorMessage, &errorName);


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
               // Also sends the name to the server and acquires games list
               RequestConstructor reqConstructor("ReqGetGamesList");
               if (userName.empty())
               {
                  int randomID = RandomIdGenerator::generateUniqueId();
                  userName = "Anon_Player";
                  userName.append(std::to_string(randomID));
                  reqConstructor.appendItem("misc", userName);
                  auto json_string = reqConstructor.ConstructRequest();
                  GetGamesList getGamesList = GetGamesList(json_string);
                  wrapper.sendReq(constants::ReqType::GETGAMES, getGamesList, client);
                  view_state = 1;
               }
               else {
                  reqConstructor.appendItem("misc", userName);
                  auto json_string = reqConstructor.ConstructRequest();
                  GetGamesList getGamesList = GetGamesList(json_string);
                  wrapper.sendReq(constants::ReqType::GETGAMES, getGamesList, client);
                  view_state = 1;
               }
            }),
        }) | flex,
     });


     auto page = Container::Vertical({
          page0 | Maybe([&] {return pagenum == 0;}),
     });

     return page;
}
}