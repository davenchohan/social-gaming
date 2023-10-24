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
    ParserLibraryInterface interface;
    std::string someString = R"({"gameRequest":"reqCreateGame"})";
    std::string someString2 = "{\"gameRequest\":\"reqCreateGame\"}";
    Json item = interface.parseJson(someString);
    std::cout << item["gameRequest"] << std::endl;
    Json item2 = interface.parseJson(someString2);
    std::cout << item2["gameRequest"] << std::endl;

    return 0;
}