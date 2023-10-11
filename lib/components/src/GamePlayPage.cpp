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
#include "GamePlayPage.h"

using namespace ftxui;

namespace Pages{

Component GamePlay(networking::Client &client){
    auto text = Renderer([] {
        return paragraph("Game Play Screen");
    });
    auto page = Container::Vertical({
        text,
    });

    return page;
}
}


