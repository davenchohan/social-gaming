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

  class InputData{
  public:
  
  InputData() = default;
  

  void addInputField(std::string key, std::string& value){
    inputFields.insert({key, value});
  }

 

  std::string toJson(){
    std::string result;
    /**
     * IE -
     *  {"key":"value", "key2":"value2"}
    */  
   //This is not really perfect will need testing
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
  std::map<std::string, std::string&> inputFields;
};

using namespace ftxui;
namespace Pages{
    Component CreateGameSession(int &pagenum, std::string &session_name, std::vector<std::string> &radiobox_list, int &radiobox_selected, int &view_state, std::string &input_num_rounds,  networking::Client &client);
}
