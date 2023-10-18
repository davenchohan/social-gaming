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
#include "JoinGamePage.h"
#include "DataClasses.h"
#include "ClientWrapper.h"
#include "Constants.h"

using namespace ftxui;

namespace Pages{
Component MobileLandingPage(int &pagenum, std::string &code, std::string &name, networking::Client &client){
    const int max_pagenum = 1; // starting 0
    networking::ClientWrapper wrapper;
    
    auto title1 = Renderer([] {
        return paragraph("Invite Code");
    });
    auto title2 = Renderer([] {
        return paragraph("Display Name");
    });
    auto page1 = Container::Vertical({
        title1,
        Input(&code, "Enter invite code to join a game session."),
    });
    auto page2 = Container::Vertical({
        title2,
        Input(&name, "Enter a displayed name."),
    });
    auto content = Container::Vertical({
        page1 | Maybe([&] {return pagenum == 0;}),
        page2 | Maybe([&] {return pagenum == 1;}),
        Renderer([] {
               return filler();
            }),
        Container::Horizontal({
            Button("Back", [&]{
            pagenum--;
            }) | Maybe([&] {return pagenum > 0;}),
            Renderer([] {
               return filler();
            }),
            Button("Next", [&]{
                wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);
            pagenum++; // todo: need to check if invite code entered/valid before proceeding
            }) | Maybe([&] {return pagenum < max_pagenum;}),
            Button("Join", [&]{
                GetGame getGame = GetGame(code);
                // TODO: replace endpoint with JOINGAME once implemented at backend
                wrapper.sendReq(constants::ReqType::DEMOGETGAME, getGame,client);
                // transfer page to game play page
            }) | Maybe([&] {return pagenum == max_pagenum;}),
        }) | flex,
    });
    return content;
}
}