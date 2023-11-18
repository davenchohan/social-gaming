#include "TestGameComponent.h"
#include "ClientWrapper.h"

using namespace ftxui;

namespace Pages{
    /*
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
};
*/

Component TestGamePage(std::vector<std::vector<std::string>> &values, std::vector<int> &selected_items, std::vector<Element> &text_list,
    Elements &texts, std::vector<std::string> &options, int &selection, networking::Client &client) {
    // auto game_selector = Radiobox(&values[0], &selected_items[0]);
    // auto block = Container::Vertical({
    //     selector,
    //     Renderer([&] {
    //         return paragraph("First selected item: ");
    //     }),
    //     Renderer([&] {
    //         return paragraph(std::to_string(selected_items[0]));
    //     }),
    //     Renderer([&] {
    //         return paragraph("Second selected item: ");
    //     }),
    //     Renderer([&] {
    //         return paragraph(std::to_string(selected_items[1]));
    //     }),
    //     Renderer([&] {
    //         return vbox(text_list);
    //     }),
    // });
    // networking::ClientWrapper wrapper;

    auto selector = Radiobox(&options, &selection);
    auto block = Container::Vertical({
        Renderer([&] {
            return vbox(texts);
        }),
        selector,
        Container::Horizontal({
            Button("Submit Selection", [&] {
                // GetGame getGame = GetGame("252434");
                // wrapper.sendReq(constants::ReqType::GETGAME, getGame, client);
            }),
        }),
    });
    return block;
}

}