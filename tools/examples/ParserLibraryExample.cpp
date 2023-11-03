#include "CustomExceptions.h"
#include "Player.h"
#include "Game.h"
#include "GameSessionHandler.h"
#include "GameVariable.h"
#include "AudienceMember.h"
#include "GameList.h"
#include "ParserLibrary.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main(){
    RequestConstructor reqConstructor("ReqCreateGame");
    reqConstructor.appendItem("name", "Gabe");
    reqConstructor.appendItem("name", "Peter");
    reqConstructor.appendItem("GameName", "Rock, Paper, Scissors");
    JsonConverter converter;

    Json item;
    item["favFoods"] = {"Burgers", "Fries", "Dumplings"};
    item["favDrinks"] = {"Water", "Apple Juice", "Coke"};
    reqConstructor.appendItem("misc", item);
    cout << "-----------------------------" << endl;
    std::string str = reqConstructor.ConstructRequest();
    cout << "Printing raw string value of constructed request: " << endl;
    std::cout << str << std::endl;
    cout << "-----------------------------" << endl;

    cout << "-----------------------------" << endl;
    cout << "Example use cases for Request Constructor" << endl;
    Json item2 = converter.GetJsonItem(str);
    if(item2.find("name") != item2.end()){
        std::cout << "Found: " + item2["name"].dump() << std::endl;
    }
    std::map<std::string, int> ps = {{"Gabe",1234}, {"John",6054}};
    reqConstructor.appendItem("Players", ps);
    RequestInfo item3 = reqConstructor.returnReqInfo();
    cout << "Request: " << item3.request << endl;
    cout << "GameName: " << item3.gameName << endl;
    cout << "GameID: " << item3.gameID << endl;
    cout << "Players: " << endl;
    for (const auto &ptr : item3.players){
        cout << ptr.first << " : " << ptr.second << ", ";
    }
    cout << endl;
    cout << "misc: " << item3.misc.dump() << endl;
    cout << "-----------------------------" << endl;
    cout << "-----------------------------" << endl;
    
    cout << "Example use cases for Request Parser" << endl;
    RequestParser parser(str);
    auto info = parser.getRequestStruct();
    cout << "Parsed from a string: " << endl;
    cout << "Request: " << info.request << endl;
    cout << "-----------------------------" << endl;
    cout << "-----------------------------" << endl;

    Json pp;
    Json state = Player::Active;
    pp["name"] = "Gabe";
    pp["id"] = 1234;
    pp["playerState"] = state;

    Player convertedItem = converter.ConvertToPlayer(pp);
    cout << convertedItem.GetName() << endl;
    cout << convertedItem.GetUserId() << endl;
    cout << convertedItem.GetPlayerState() << endl;
    return 0;
}