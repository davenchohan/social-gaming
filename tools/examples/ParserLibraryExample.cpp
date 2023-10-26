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
    RequestParser reqParser("ReqCreateGame");
    reqParser.appendRequest("name", "Gabe");
    reqParser.appendRequest("name", "Peter");
    reqParser.appendRequest("GameName", "Rock, Paper, Scissors");

    Json item;
    item["favFoods"] = {"Burgers", "Fries", "Dumplings"};
    item["favDrinks"] = {"Water", "Apple Juice", "Coke"};
    reqParser.appendRequest("misc", item);
    std::string str = reqParser.ConstructRequest();
    std::cout << str << std::endl;
    Json item2 = getJsonItem(str);
    if(item2.find("name") != item2.end()){
        std::cout << "Found: " + item2["name"].dump() << std::endl;
    }
    std::map<std::string, int> ps = {{"Gabe",1234}, {"John",6054}};
    reqParser.appendRequest("Players", ps);
    RequestInfo item3 = reqParser.returnReqInfo();
    cout << "Request: " << item3.request << endl;
    cout << "GameName: " << item3.gameName << endl;
    cout << "GameID: " << item3.gameID << endl;
    cout << "Players: " << endl;
    for (const auto &ptr : item3.players){
        cout << ptr.first << " : " << ptr.second << ", ";
    }
    cout << endl;
    cout << "misc: " << item3.misc.dump() << endl;
    return 0;
}