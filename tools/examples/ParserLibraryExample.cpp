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
    /*
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
    std::vector<Player*> players;
    Player player1("Gabe", 1234);
    Player player2("John", 1233);
    Player player3("Michael", 4444);
    players.push_back(&player1);
    players.push_back(&player2);
    players.push_back(&player3);
    std::string pl_str = "Players";
    reqConstructor.appendItem(pl_str, players);
    auto json_string = reqConstructor.ConstructRequest();
    cout << json_string << endl;
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

    std::vector<Player *> players;
    Player player1("Gabe", 1234);
    Player player2("John", 1233);
    Player player3("Michael", 4444);
    players.push_back(&player1);
    players.push_back(&player2);
    */
    RequestConstructor reqConstructor("ReqCreateGame");
    reqConstructor.appendItem("name", "Gabe");
    reqConstructor.appendItem("name", "Peter");
    reqConstructor.appendItem("GameName", "Rock, Paper, Scissors");
    std::vector<Player> players;
    Player player1("Gabe", 1234);
    Player player2("John", 1233);
    Player player3("Michael", 4444);
    players.push_back(player1);
    players.push_back(player2);
    players.push_back(player3);
    std::string pl_str = "Players";
    reqConstructor.appendItem(pl_str, players);
    RequestInfo item = reqConstructor.returnReqInfo();
    cout << "Printing player:" << endl;
    cout << item.players.front().GetName() << endl;
    

    auto json_string = reqConstructor.ConstructRequest();
    cout << json_string << endl;  
    RequestParser parser(json_string);


    return 0;
}