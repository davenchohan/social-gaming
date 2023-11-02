#include "ParserLibrary.h"

RequestInfo
RequestConstructor::returnReqInfo(){
    RequestInfo temp;
    subject.at("Request").get_to(temp.request);
    subject.at("GameName").get_to(temp.gameName);
    subject.at("GameID").get_to(temp.gameID);
    temp.gameConfig = subject.at("GameConfig");
    temp.players= subject.at("Players").get_to(temp.players);
    temp.misc = subject.at("misc");
    return temp;
}

// Sets json item to blank classes, as returnReqInfoFromSubject can be called anytime
// get_to throws an error if item is null
RequestConstructor::RequestConstructor(std::string item){
    Json blankJson;
    std::map<std::string, int> blankPlayers;
    subject["Request"] = item;
    subject["GameName"] = "";
    subject["GameID"] = "";
    subject["GameConfig"] = blankJson;
    subject["Players"] = blankPlayers;
    subject["misc"] = blankJson;
}

std::string RequestConstructor::ConstructRequest(){
    return subject.dump();
}

// Takes a message received from server and sets the request subject as the parsed JSon item
// logStr should be in string format 
RequestParser::RequestParser(std::string& logStr){
    subject = Json::parse(logStr);
}


RequestInfo
RequestParser::getRequestStruct(){
    RequestInfo temp;
    subject.at("Request").get_to(temp.request);
    subject.at("GameName").get_to(temp.gameName);
    subject.at("GameID").get_to(temp.gameID);
    temp.gameConfig = subject.at("GameConfig");
    temp.players= subject.at("Players").get_to(temp.players);
    temp.misc = subject.at("misc");
    return temp;
}

Json getJsonItem(const std::string item){
    Json j = Json::parse(item);
    return j;
}


