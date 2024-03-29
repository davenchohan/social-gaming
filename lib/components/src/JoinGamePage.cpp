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
Component JoinGame(int &pagenum, std::string &code, std::string &name, networking::Client &client){
    const int max_pagenum = 1; // starting 0
    auto title1 = Renderer([] {
        return paragraph("Invite Code");
    });
    auto title2 = Renderer([] {
        return paragraph("Attempting to Join Game...");
    });
    auto page1 = Container::Vertical({
        title1,
        Input(&code, "Enter invite code to join a game session."),
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
            pagenum++; // todo: need to check if invite code entered/valid before proceeding
            }) | Maybe([&] {return pagenum < max_pagenum;}),
            Button("Join", [&]{
                // transfer page to game play page
            }) | Maybe([&] {return pagenum == max_pagenum;}),
        }) | flex,
    });
    auto page2 = Container::Vertical({
        title2,
        //Input(&name, "Enter a displayed name."),
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
            pagenum++; // todo: need to check if invite code entered/valid before proceeding
            }) | Maybe([&] {return pagenum < max_pagenum;}),
            Button("Join", [&]{
                // transfer page to game play page
            }) | Maybe([&] {return pagenum == max_pagenum;}),
        }) | flex,
    });
    auto content = Container::Vertical({
        page1 | Maybe([&] {return pagenum == 0;}),
        page2 | Maybe([&] {return pagenum == 1;}),
    });
    // auto page = Window({.title = "Join Game",.width = 80, .height = 60,});
    return content;
}
}