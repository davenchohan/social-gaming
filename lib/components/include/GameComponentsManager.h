#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../../networking/include/Client.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" 
#include "Constants.h"
#include "GameComponent.h"

using namespace gameComponent;


class GameComponentsManager {
    void addGameComponent();
    void clearAllGameComponents();

    private:
    std::vector<GameComponent> collection;
}