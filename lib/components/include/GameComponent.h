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

namespace gameComponents {
struct ComponentData {
    std::vector<std::string> values;// radiobox or paragraphs
    std::string text; //input
    int selected; //radiobox
};

class GameComponent {
    public:
    class ComponentGenerator {
        public:
        Component createDisplayComponent(std::vector<std::string> &values);

        // returns a component with a radiobox
        Component createSingleSelector(std::vector<std::string> &values, int &selected);

        // returns a component with a checkbox
        // Component createMultiSelector();

        // returns a component with an input
        Component createInput(std::string &input);
    };
    GameComponent(constants::GameComponentType type, ComponentData &data): type(type);

    private:
    constants::GameComponentType type;
    Component block;// should this be reference?
    ComponentData data;
}
}