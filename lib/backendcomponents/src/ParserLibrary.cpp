#include "ParserLibrary.h"

RequestInfo
Parser::returnReqInfoFromSubject(){
    RequestInfo temp;
    subject.at("Request").get_to(temp.request);
    subject.at("GameName").get_to(temp.gameName);
    subject.at("GameID").get_to(temp.gameID);
    temp.gameConfig = subject.at("GameConfig");
    temp.players= subject.at("Players").get_to(temp.players);
    temp.misc = subject.at("misc");
    return temp;
}

Parser::Parser(std::string key, std::string item){
    Json blankConfig;
    std::vector<std::string> blankPlayers;
    Json blankMisc;
    subject[key] = item;
    subject["GameName"] = "";
    subject["GameID"] = "";
    subject["GameConfig"] = blankConfig;
    subject["Players"] = blankPlayers;
    subject["misc"] = blankMisc;
}

std::string Parser::subjectToString(){
    return subject.dump();
}

RequestParser::RequestParser(std::string req){
    Parser pReq("Request", req);
    this->request = pReq;
}

std::string RequestParser::ConstructRequest(){
    return request.subjectToString();
}

RequestInfo
RequestParser::returnReqInfo(){
    return request.returnReqInfoFromSubject();
}


Json getJsonItem(const std::string item){
    Json j = Json::parse(item);
    return j;
}


