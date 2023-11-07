#include "ParserLibrary.h"


//Appends each item from RequestInfo struct into the json object
RequestConstructor::RequestConstructor(RequestInfo &info){
    appendItem("Request", info.request);
    appendItem("GameName", info.gameName);
    appendItem("GameID", info.gameID);
    appendItem("GameConfig", info.gameConfig);
    appendItem("Players", info.players);
    appendItem("misc", info.misc);
}

// Sets json item to blank classes, as returnReqInfoFromSubject can be called anytime
RequestConstructor::RequestConstructor(std::string request){
    Json blankJson;
    std::map<std::string, int> blankPlayers;
    subject["Request"] = request;
    subject["GameName"] = "";
    subject["GameID"] = "";
    subject["GameConfig"] = blankJson;
    subject["Players"] = blankPlayers;
    subject["misc"] = blankJson;
}

std::string RequestConstructor::ConstructRequest(){
    return subject.dump();
}

RequestInfo 
RequestConstructor::returnReqInfo(){
    RequestInfo temp;
    subject.at("Request").get_to(temp.request);
    subject.at("GameName").get_to(temp.gameName);
    subject.at("GameID").get_to(temp.gameID);
    temp.gameConfig = subject.at("GameConfig");
    auto players_arr = subject.at("Players");
    converter.convertJsonToPlayersArr(players_arr, temp.players);
    temp.misc = subject.at("misc");
    return temp;
}

// Takes a message received from server and sets the request subject as the parsed JSon item
// logStr should be in string format 
RequestParser::RequestParser(std::string& logStr){
    subject = Json::parse(logStr);
}

// Expected formats:
RequestInfo
RequestParser::getRequestStruct(){
    RequestInfo retStruct;
    subject.at("Request").get_to(retStruct.request);
    subject.at("GameName").get_to(retStruct.gameName);
    subject.at("GameID").get_to(retStruct.gameID);
    retStruct.gameConfig = subject.at("GameConfig");
    converter.convertJsonToPlayersArr(subject.at("Players"), retStruct.players);
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

Json JsonConverter::ConvertFromPlayer(Player player){
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
    /*
    std::string varName;
    std::string varVal;
    item.at("variableName").get_to(varName);
    item.at("variableValue").get_to(varVal);
    */
   int val = 0;
   return GameVariable{val};
}

void
RequestConstructor::appendItem(const std::string key, std::vector<Player> players){
    std::cout << "app2 called " << std::endl;
    /*
    Input(s):
    - Key: Used to verify if the key is players
    - vector of Player pointers to be added as a json array
    */
   std::string oracle = "Players";
    if(key != oracle){
        std::cout << "Error, supplied a vector of Players but key is: " << key << std::endl;
        return;
    }else{
        JsonConverter converter;
        std::vector<Json> playersArr;
        std::for_each(players.begin(), players.end(), [&playersArr, &converter](auto &item){
            Json jitem = converter.ConvertFromPlayer(item);
            playersArr.push_back(jitem);
        });
        subject["Players"] = playersArr;
        return;
    }
}

void JsonConverter::convertJsonToPlayersArr(Json &arr, std::vector<Player> &p_vector){
    if(arr.empty()){
        std::cout << "Empty array passed" << std::endl;
        return;
    }else{
        std::for_each(arr.begin(), arr.end(), [&p_vector, this](auto &item){
            Player temp = this->ConvertToPlayer(item);
            p_vector.push_back(temp);
        });
        return;
    }
}