#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Client.h"
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp"
#include "ClientWrapper.h"
#include "Constants.h"
#include "GameConfigPage.h"



class InputData{
  public:
  std::map<std::string, std::string&> inputFields;


  void addInputField(std::string key, std::string& value){
    inputFields.insert({key, value});
  }

  std::string toJson(){
    std::string result;
    /**
     * IE -
     *  {"key":"value", "key2":"value2"}
    */  
    result.append("{");
    for(auto const& [key, val] : inputFields){
      result.append(key);
      result.append(":");
      result.append(val);
      result.append(", ");
    }
    result.append("}");
    return result;
  }
  private:
};

//TODO: make this load after hitting next on game selection page.

//TODO: look into bug where demo list loads before we receive list from server
using namespace ftxui;
namespace Pages
{
Component GameConfig(int &create_pagenum, std::string &session_name, std::vector<std::string> &radiobox_list, int &radiobox_selected, networking::Client &client){

  networking::ClientWrapper wrapper;
  //wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);

  InputData data;  
  

  //textFields for input, need some way to define which type of input.
  //doing a demo of config for a quiz game, was not sure what input rock paper scissors would need.
  //once I get server responding will make this dynamic.
  std::string input_num_rounds;
  
 
  Component num_rounds = Input(&input_num_rounds, "Number of Rounds");
  

  data.addInputField("num_rounds", input_num_rounds);
  

  


  auto configPage = Container::Vertical({
    num_rounds,
    Renderer([] {
      return filler();
    }),
    //buttons to either cancel and return to game select screen or send input to server as JSON
    Container::Horizontal({
      Button("Back", [&]{
        wrapper.sendNoBody(constants::ReqType::DEMOGETGAMES, client);
        //TODO:Make this return to game select screen
      }),
      Renderer([] {
      return filler();
      }),
      Button("Create", [&]{
        
        
        
        
        //TODO:verify this is correct
        //wrapper.sendReq(constants::ReqType::STARTGAME, data.toJSON(), client);
        
        
        

      })
    })
  });
  


     
  return configPage;
}
}

