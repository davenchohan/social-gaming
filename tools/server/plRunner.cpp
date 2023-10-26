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

int main(){
    RequestParser reqParser("ReqCreateGame");
    reqParser.appendRequest("name", "Gabe");
    reqParser.appendRequest("name", "Peter");
    Json item;
    item["favFoods"] = {"Burgers", "Fries", "Dumplings"};
    item["favDrinks"] = {"Water", "Apple Juice", "Coke"};
    reqParser.appendRequest("someStuff", item);
    std::string str = reqParser.ConstructRequest();
    std::cout << str << std::endl;
    Json item2 = getJsonItem(str);
    if(item2.find("name") != item2.end()){
        std::cout << "Found: " + item2["name"].dump() << std::endl;
    }

    RequestInfo item3 = reqParser.returnReqInfo();
    return 0;
}