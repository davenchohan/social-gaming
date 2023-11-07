
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
#include "TestGameComponent.h"


using namespace ftxui;

// implementation temporarily in header file (needs to be moved to GameComponent.cpp)
class ComponentData {// more like ComponentManager
    public:
    ComponentData() {}
    // void addComponent(const std::vector<std::string> str) {
    //     // store data
    //     addElements(str);

    //     // create component
    //     Component c = createParagraphBlock(elms_collection.back());

    //     // store the component
    //     // cmps.push_back(c);
    // }
    void addComponent(const std::vector<std::string> &str) {
        data_collection.push_back(str);
        selections.push_back(0);
        createRadioboxBlock(data_collection.back(), selections.back());
    }
    Components getComponents() {
        return cmps;
    }
    std::string numberOfComponents() {
        return std::to_string(cmps.size());
    }

    private:
    // Component createParagraphBlock(std::vector<std::shared_ptr<Element>> &paragraphs) {
    // // Component createParagraphBlock(std::vector<Element> &paragraphs) {
    //     auto block = Container::Vertical({
    //         Renderer([&] {
    //             return vbox(paragraphs);
    //         }),
    //     });
    //     cmps.push_back(block);
    //     return block;
    // }

    Component createParagraphBlock(std::vector<std::shared_ptr<Element>> &paragraphs) {
        auto block = Container::Vertical({
            Renderer([&] {
                return paragraph("Empty");
            }) | Maybe([&] {return paragraphs.empty();}),
            Renderer([&] {
                std::vector<Element> elements;
                for (const auto& paragraph_ptr : paragraphs) {
                    elements.push_back(*paragraph_ptr);
                }
                return vbox(elements);
                // return vbox(paragraphs);
            }),
            Renderer([&] {
                return paragraph(std::to_string(paragraphs.size()));
            }),

        });
        cmps.push_back(block);
        return block;
    }

    Component createRadioboxBlock(std::vector<std::string> &values, int &selection) {
        auto block = Radiobox(&values, &selection);
        cmps.push_back(block);
        return block;
    }

    void addElements(const std::vector<std::string> &dataset) {
        std::vector<std::shared_ptr<Element>> values;
        for(auto data : dataset) {
            values.push_back(std::make_shared<Element>(paragraph(data)));
        }

        elms_collection.push_back(values);
    }

    std::vector<std::vector<std::shared_ptr<Element>>> elms_collection;
    std::vector<std::vector<std::string>> data_collection;
    std::vector<int> selections;
    Components cmps;
};