#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include "../../networking/include/Client.h"
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" 
#include "Constants.h"
#include "GameComponent.h" // struct ComponentData

using namespace ftxui;
namespace Pages{
Component ParagraphBlock(std::vector<Element> &paragraphs);
Component ParagraphBlockPointerVer(std::vector<std::shared_ptr<Element>> &paragraphs);
// Component TestGamePage(std::vector<ComponentData> &data, networking::Client &client);
Component TestGamePage(std::vector<std::vector<std::string>> &values, std::vector<int> &selected_items, std::vector<Element> &text_list); 
}
/*
class ComponentGenerator {
    public:
    Component createDisplayComponent(const std::vector<std::string> &values);

    // returns a component with a radiobox
    // Component createSingleSelector(const std::vector<std::string> &values, int &selected);

    // returns a component with a checkbox
    // Component createMultiSelector();

    // returns a component with an input
    // Component createInput(std::string &input);
};

class TestGameComponent {
    public:
    TestGameComponent(constants::GameComponentType type, ComponentGenerator generator, std::vector<std::string> &values, std::string &text, int selected);
    Component getGameComponent();

    private:
    Component block;
    std::vector<std::string> values;// radiobox or paragraphs
    std::string text; //input
    int selected; //radiobox
};
*/