#include "TestGameComponent.h"

using namespace ftxui;

namespace Pages{
Component TestGamePage(std::vector<ComponentData> &data, networking::Client &client) {
    // std::vector<Component> paragraphs;
    // auto p = Renderer([&]() {
    //     return paragraph("Placeholder");
    // });
    // std::vector
    Elements paragraphs;

    for(size_t i = 0; i < data[0].values.size(); i++) {
        paragraphs.push_back(
            hbox({
                paragraph(data[0].values[i]),
            })
        );
        // paragraphs.push_back(Renderer([&] {
        //     return paragraph(data[0].values[i]);
        // }));
    }


    // didn't work
    // for(auto value: data[0].values) {
    //     paragraphs.push_back(Renderer([&] {
    //         return paragraph(value);
    //     }));
    // }

    // if(data.size() > 0 && data[0].values.size() > 0) {
    //     // if(data[0].values[0] == "dummy1") {
    //     //     p = Renderer([&] {
    //     //         return paragraph("Expecting: dummy1");
    //     //     });
    //     // }
    //     p = Renderer([&] {
    //         return paragraph(data[0].values[0]);
    //     });
    //     paragraphs.push_back(Renderer([&] {
    //         return paragraph(data[0].values[0]);
    //     }));
    // }

    auto content = Container::Vertical({
        Renderer([&] {
            return paragraph(data[0].values[0]);
        }),
        Renderer([&] {
            return paragraph(data[0].values[1]);
        }),
        Renderer([&] {
            return paragraph(data[0].values[2]);
        }),
    });
    // auto content = Container::Vertical({
    //     p | color(Color::GreenLight),
    // });
    return content;
}

// Component ComponentBlock(constants::GameComponentType type, ::vector<std::string> &values, int &selected) {
//     // if(type == constants::GameComponentType::DISPLAY) {}
// }
}