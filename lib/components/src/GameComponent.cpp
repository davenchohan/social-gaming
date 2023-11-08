#include "GameComponent.h"

using namespace ftxui;
/*
ComponentData::ComponentData(std::vector<std::string> &values, std::string &text, int selected): values(values), text(text), selected(selected) {}
const std::vector<std::string>& ComponentData::getValues() {
    return values;
}
std::string& ComponentData::getText() {
    return text;
}
int& ComponentData::getSelected() {
    return selected;
}
// namespace ui {
GameComponent::GameComponent(constants::GameComponentType type, ComponentData &data): type(type), data(data) {
    GameComponent::ComponentGenerator generator;
    switch (type) {
        case constants::GameComponentType::DISPLAY:
            block = generator.createDisplayComponent(data.getValues());
            break;
        case constants::GameComponentType::SINGLE_SELECT:
            block = generator.createSingleSelector(data.getValues(), data.getSelected());
            break;
        // case constants::GameComponentType::MULTI_SELECT:
        //     break;
        case constants::GameComponentType::INPUT:
            block = generator.createInput(data.getText());
            break;
        
        default:
            block = Container::Vertical({
                Renderer([] {
                    return filler();
                }),
            });
            break;
    }
}

Component GameComponent::getGameComponent() {
    return block;
}

Component GameComponent::ComponentGenerator::createDisplayComponent(const std::vector<std::string> &values) {
    std::vector<Component> paragraphs;
    paragraphs.reserve(values.size());
    for(auto value: values) {
        auto p = Renderer([&value] {
            return paragraph(value);
        });
        paragraphs.push_back(p);
    }

    auto block = Container::Vertical(paragraphs);

    // auto selectedGameTitle = Renderer([&] {
    //       return paragraph("Selected game: " + radiobox_list[radiobox_selected]);
    //  });
    return block;
}


Component GameComponent::ComponentGenerator::createSingleSelector(const std::vector<std::string> &values, int &selected) {
    auto radiobox = Radiobox(&values, &selected);
    auto block = Container::Vertical({
        radiobox,
    });
    return block;
}


// Component GameComponent::createMultiSelector();

Component GameComponent::ComponentGenerator::createInput(std::string &input) {
    auto inputField = Input(&input, "Enter invite code to join a game session.");
    auto block = Container::Vertical({
        inputField,
    });
    return block; 
}
// }
*/