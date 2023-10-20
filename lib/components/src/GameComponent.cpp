#include "GameComponent.h"

GameComponent::GameComponent(constants::GameComponentType type, gameComponents::ComponentData &data): type(type) {
    ComponentGenerator::GameComponent::ComponentGenerator generator;
    switch (type) {
        case constants::GameComponentType::DISPLAY:
            block = generator.createDisplayComponent(data.values);
            break;
        case constants::GameComponentType::SINGLE_SELECT:
            block = generator.createSingleSelector(data.values, data.selected);
            break;
        // case constants::GameComponentType::MULTI_SELECT:
        //     break;
        case constants::GameComponentType::INPUT:
            block = generator.createInput(data.text);
            break;
        
        default:
            block = Container::Vertical({
                filler(),
            });
            break;
    }
}

Component GameComponent::ComponentGenerator::createDisplayComponent(std::vector<std::string> &values) {
    std::vector<Element> paragraphs;
    paragraphs.reserve(values.size());
    for(auto value: values) {
        auto paragraph = paragraph(value);
        paragraphs.push_back(paragraph);
    }

    auto block = Container::Vertical(paragraphs);

    // auto selectedGameTitle = Renderer([&] {
    //       return paragraph("Selected game: " + radiobox_list[radiobox_selected]);
    //  });
    return block;
}


Component GameComponent::ComponentGenerator::createSingleSelector(std::vector<std::string> &values, int &selected) {
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