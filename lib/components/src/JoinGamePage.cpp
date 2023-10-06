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

using namespace ftxui;

namespace Pages{
Component JoinGame(int &pagenum, std::string &code, std::string &name, bool &showLanding, bool &showJoin, bool &showCreate, networking::Client &client){
    const int max_pagenum = 1; // starting 0
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
        Container::Horizontal({
            Button("Back", [&]{
            pagenum--;
            }) | Maybe([&] {return pagenum > 0;}),
            Button("Next", [&]{
            pagenum++; // todo: need to check if invite code entered/valid before proceeding
            }) | Maybe([&] {return pagenum < max_pagenum;}),
            // }) | Maybe([&] {return pagenum < max_pagenum;}),
            Button("Join", [&]{}) | Maybe([&] {return pagenum == max_pagenum;}),// transfer page to game play page
        }) | flex,
    });
    // auto page = Window({.title = "Join Game",.width = 80, .height = 60,});
    return content;
}
}