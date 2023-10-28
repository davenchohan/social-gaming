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
#include "GameComponent.h"
#include "Constants.h"

using namespace ftxui;
// using namespace ui;

namespace Pages{
Component GamePlay(int &view_state, GameComponent &gameComponent, networking::Client &client) {
    auto text = Renderer([] {
        return paragraph("Testing Component Generation");
    });
    auto content = Container::Vertical({
        text,
        Button("Return non-game-play view (state 0)", [&]{
            view_state = 0;
        }),
        gameComponent.getGameComponent(),
    });
    return content;
}
}