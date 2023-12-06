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
Component loadingPage(int &pagenum, int &view_state, networking::Client &client) {

     //const int max_pagenum = 0;// starting 0
     //auto game_selector = Radiobox(&radiobox_list, &radiobox_selected);

     auto title1 = Renderer([] {
          return paragraph("Loading...") |
          center |
          bold |
          color(Color::Cyan);
     });

     // pages
     auto page0 = Container::Vertical({
          title1,
          Renderer([] {
               return filler();
            }),
          Container::Horizontal({
            Renderer([] {
               return filler();
            }),
        }) | flex,
     });


     auto page = Container::Vertical({
          page0 | Maybe([&] {return pagenum == 0;}),
     });

     // auto page = Window({.inner = game_selector, .title = "Create Game session",.width = 80, .height = 60});
     return page;
}
}