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

#include "MainPageContent.h"
#include "ClientWrapper.h"
#include "Constants.h"
#include "ParserLibrary.h"

using namespace ftxui;

namespace Pages{

Component MainPageContent(
                        std::string& state, 
                        std::string& description, 
                        std::string& type, 
                        std::vector<std::string>& options, 
                        int& selection, 
                        std::string& prompt, 
                        std::string& input, 
                        std::string& buttonLabel, 
                        std::string& endpoint,
                        networking::Client& client) {

  networking::ClientWrapper wrapper;
  RequestConstructor constructor(endpoint);


  auto state_ = Renderer([state] { return paragraph(state); });
  auto description_ = Renderer([description] { return paragraph(description); });
  auto type_ = Renderer([type] { return paragraph(type); });
  auto selector = Radiobox(&options, &selection);
  auto prompt_ = Renderer([prompt] { return paragraph(prompt); });
  auto input_ = Input(&input, "Enter here");
  auto button_ = Button(buttonLabel, [&] {
    // constructor.appendItem("GameName", options[selection]);
    // auto json_string = constructor.ConstructRequest();
    // GetGameName getGameName = GetGameName(json_string);
    // wrapper.sendReq(endpoint, getGameName, client);
  });



  // auto buttons = Container::Horizontal({
  //   Button("Back", [&] {
  //   }),
  //   Renderer([] {
  //       return filler();
  //   }),
  //   button_,
  // });

  // auto renderer = Renderer(buttons, [&] {
  //   return vbox({
  //     hbox({
  //       paragraph(state),
  //     }),
  //     hbox({
  //       buttons->Render(),
  //     }),
  //   });
  // });


  // auto content = Container::Vertical({
  //   state_,
  //   description_,
  //   type_,
  //   selector,
  //   prompt_,
  //   input_,
  // });


  auto content = Container::Vertical({
    Renderer([] { return paragraph("state"); }),
    state_,
    Renderer([] { return paragraph("description"); }),
    description_,
    Renderer([] { return paragraph("type"); }),
    type_,
    Renderer([] { return paragraph("selector"); }),
    selector,
    Renderer([] { return paragraph("prompt"); }),
    prompt_,
    Renderer([] { return paragraph("input"); }),
    input_,
    Container::Horizontal({
      Button("Back", [&] {
      }),
      Renderer([] {
          return filler();
      }),
      button_,
    }),
  });

  return content;
}
}


