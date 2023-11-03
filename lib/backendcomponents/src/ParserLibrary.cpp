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

// Expected formats:
// 
RequestInfo
RequestParser::getRequestStruct(){
    RequestInfo retStruct;
    subject.at("Request").get_to(retStruct.request);
    subject.at("GameName").get_to(retStruct.gameName);
    subject.at("GameID").get_to(retStruct.gameID);
    retStruct.gameConfig = subject.at("GameConfig");
    retStruct.players= subject.at("Players").get_to(retStruct.players);
    retStruct.misc = subject.at("misc");
    return retStruct;
}

Json JsonConverter::GetJsonItem(const std::string item){
    return Json::parse(item);
}

Json JsonConverter::ConvertFromUser(User& user){
    Json item;
    item["name"] = user.GetName();
    item["id"] = user.GetUserId();
    return item;
}

// Single instance of a user class in Json format
// Example: { "name":"Gabe", "Id":1234}
User JsonConverter::ConvertToUser(const Json &item){
    std::string name;
    int id;
    item.at("name").get_to(name);
    item.at("id").get_to(id);
    return User{name, id};
}

Json JsonConverter::ConvertFromPlayer(Player &player){
    Json item;
    item["name"] = player.GetName();
    item["id"] = player.GetUserId();
    item["playerState"] = player.GetPlayerState();
    return item;
}

// Input: Single instance of a player class in Json Format
// Example: { "name":"Gabe", "Id":1234, "playerState":"Active"}
Player JsonConverter::ConvertToPlayer(const Json &item){
    std::string name;
    int id;
    item.at("name").get_to(name);
    item.at("id").get_to(id);
    Player::PlayerState state = item.at("playerState").template get<Player::PlayerState>();
    Player player(name, id);
    player.SetPlayerState(state);
    return player;
}

Json JsonConverter::ConvertFromAudienceMember(AudienceMember& member){
    Json item;
    item["name"] = member.GetName();
    item["id"] = member.GetUserId();
    item["audienceMemberState"] = member.GetAudienceState();
    return item;
}

// Input: Single instance of a audience member class in Json Format
// Example: { "name":"Gabe", "Id":1234, "audienceMemberState":"Inactive"}
AudienceMember JsonConverter::ConvertToAudienceMember(const Json&item){
    std::string name;
    int id;
    item.at("name").get_to(name);
    item.at("id").get_to(id);
    AudienceMember::AudienceMemberState state = item.at("audienceMemberState").template get<AudienceMember::AudienceMemberState>();
    AudienceMember member(name, id);
    member.SetAudienceState(state);
    return member;
}

GameVariable JsonConverter::ConvertToGameVariable(const Json& item){
    std::string varName;
    std::string varVal;
    item.at("variableName").get_to(varName);
    item.at("variableValue").get_to(varVal);
    return GameVariable{varName, varVal};
}