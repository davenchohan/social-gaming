
#pragma once

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


using namespace ftxui;

// Component DisplayBlock(Elements &paragraphs) {
//     auto block = Container::Vertical({
//         Renderer([&] {
//             return vbox(paragraphs);
//         }),
//     });
//     return block;
// }

// Component SingleSelectBlock(std::vector<std::string> &values, int &selection) {
//     auto selector = Radiobox(&values, &selection);
//     auto block = Container::Vertical({
//         selector,
//     });
//     return block;
// }


/*
// namespace ui {
class ComponentData {
    public:
    ComponentData(std::vector<std::string> &values, std::string &text, int selected);
    const std::vector<std::string>& getValues();
    std::string& getText();
    int& getSelected();

    private:
    std::vector<std::string> values;// radiobox or paragraphs
    std::string text; //input
    int selected; //radiobox
};
// struct ComponentData {
//     std::vector<std::string> values;// radiobox or paragraphs
//     std::string text; //input
//     int selected; //radiobox
// };

class GameComponent {
    public:
    class ComponentGenerator {
        public:
        Component createDisplayComponent(const std::vector<std::string> &values);

        // returns a component with a radiobox
        Component createSingleSelector(const std::vector<std::string> &values, int &selected);

        // returns a component with a checkbox
        // Component createMultiSelector();

        // returns a component with an input
        Component createInput(std::string &input);
    };
    GameComponent(constants::GameComponentType type, ComponentData &data);
    Component getGameComponent();

    private:
    constants::GameComponentType type;
    Component block;// should this be reference?
    ComponentData data;
};
// }
*/

struct ComponentData {
    constants::GameComponentType type;
    std::vector<std::string> values;
    int selected;
};