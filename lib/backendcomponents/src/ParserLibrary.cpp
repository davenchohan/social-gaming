#include "ParserLibrary.h"
#include <typeinfo>
#include <limits>




//Appends each item from RequestInfo struct into the json object
RequestConstructor::RequestConstructor(RequestInfo &info){
    appendItem("Request", info.request);
    appendItem("GameName", info.gameName);
    appendItem("GameID", info.gameID);
    appendItem("GameConfig", info.gameConfig);
    appendItem("Players", info.players);
    appendItem("misc", info.misc);
    appendItem("ConnID", info.connID);
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
    subject["ConnID"] = "";
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
    subject.at("ConnID").get_to(temp.connID);
    temp.gameConfig = subject.at("GameConfig");
    auto players_arr = subject.at("Players");
    converter.convertJsonToPlayersArr(players_arr, temp.players);
    temp.misc = subject.at("misc");
    return temp;
}

// Takes a message received from server and sets the request subject as the parsed JSon item
// logStr should be in string format 
RequestParser::RequestParser(const std::string& logStr){
    subject = Json::parse(logStr);
}

std::string RequestParser::getValue(const std::string &key){
    return getValue(key, subject);
}

std::string RequestParser::getValue(const std::string &key, Json& jsonItem){
    std::string item;
    try{
        jsonItem.at(key).get_to(item);
        return item;
    }catch(...){
        return "NO_Value_Found";
    }
}

// Expected formats:
RequestInfo
RequestParser::getRequestStruct(){
    RequestInfo retStruct;
    subject.at("Request").get_to(retStruct.request);
    subject.at("GameName").get_to(retStruct.gameName);
    subject.at("GameID").get_to(retStruct.gameID);
    subject.at("ConnID").get_to(retStruct.connID);
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
   int varId;
   std::string varName;
   item.at("varId").get_to(varId);
   try{
    item.at("variableName").get_to(varName);
    std::string strVal = item.value("strVal", "");
    double doubleVal = item.value("doubleVal", std::numeric_limits<double>::min());
    int intVal = item.value("intVal", std::numeric_limits<int>::min() );
    
    // Can only have 1 of the below data types
    if (doubleVal != std::numeric_limits<double>::min()){
        return GameVariable{varName, varId, doubleVal};
    }else if (intVal != std::numeric_limits<int>::min()){
        return GameVariable{varName, varId, intVal};
    }else if(strVal != ""){ 
        return GameVariable{varName, varId, strVal};
    }else{
        std::cout << "ERROR, could not parse Game variables from: " + item.dump() + ", item did not have a valid type" << std::endl;
        return GameVariable{varId};
    }
   }
   catch(const Json::type_error &e){
    return GameVariable{varId};
   }
}

Json JsonConverter::ConvertFromGameVariable(GameVariable &var){
    Json item;
    item["variableName"] = var.GetName();
    item["varId"] = var.GetVariableId();
    if(var.IsInt()){
        item["intVal"] = var.GetData<int>();
    }else if(var.IsString()){
        item["strVal"] = var.GetData<std::string>();
    }else if(var.IsDouble()){
        item["doubleVal"] = var.GetData<double>();
    }
    return item;
}

GameConstant JsonConverter::ConvertToGameConstant(const Json& item){
    std::string name, value;
    item.at("constantName").get_to(name);
    item.at("constantVal").get_to(value);
    return GameConstant{name, value};
}

Json JsonConverter::ConvertFromGameConstant(const GameConstant &var){
    Json item;
    item["constantName"] = var.GetName();
    item["constantVal"] = var.GetConstantValue();
    return item;
}


void
RequestConstructor::appendItem(const std::string key, std::vector<Player> players){
    /*
    Input(s):
    - Key: Used to verify if the key is players
    - vector of Player to be added as a json array
    */
   std::string oracle = "Players";
    if(key != oracle){
        std::cout << "Error, supplied a vector of Players but key is: " << key << std::endl;
        return;
    }else{
        std::vector<Json> playersArr;
        std::for_each(players.begin(), players.end(), [&playersArr, this](auto &item){
            Json jitem = this->converter.ConvertFromPlayer(item);
            playersArr.push_back(jitem);
        });
        subject["Players"] = playersArr;
        return;
    }
}

void RequestConstructor::appendItem(const std::string key, std::vector<AudienceMember> members){
    /*
    Input(s):
    - Key: Used to verify if the key is players
    - vector of Audience Members to be added as a json array
    */
    std::string oracle = "AudienceMembers";
    if(key != oracle){
        std::cout << "Error, supplied a vector of Audience members but key is: " << key << std::endl;
        return;
    }else{
        std::vector<Json> membersArr;
        std::for_each(members.begin(), members.end(), [&membersArr, this](auto &item){
            Json jitem = this->converter.ConvertFromAudienceMember(item);
            membersArr.push_back(jitem);
        });
        subject["AudienceMembers"] = membersArr;
        return;
    }
    return;
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

Json JsonConverter::ConvertFromGame(const Game& game){
    Json retItem;
    retItem["GameName"] = game.GetGameName();
    retItem["GameId"] = game.GetGameId();
    retItem["MinPlayers"] = game.GetMinPlayers();
    retItem["MaxPlayers"] = game.GetMaxPlayers();
    retItem["AudienceEnabled"] = game.IsAudienceEnabled();
    retItem["NumRounds"] = game.GetNumRounds();
    retItem["GameProgress"] = game.GetGameProgress();

    std::vector<GameVariable> gameVariablesVector = game.GetAllVariables();
    std::vector<GameConstant> gameConstantsVector = game.GetAllConstants();
    std::vector<Json> jsonConstants, jsonVariables;
    std::for_each(gameConstantsVector.begin(), gameConstantsVector.end(), [&jsonConstants, this](auto &item){
        jsonConstants.push_back(this->ConvertFromGameConstant(item));
    });

    std::for_each(gameVariablesVector.begin(), gameVariablesVector.end(), [&jsonVariables, this](auto &item){
        jsonVariables.push_back(this->ConvertFromGameVariable(item));
    });
    retItem["GameConstants"] = jsonConstants;
    retItem["GameVariables"] = jsonVariables;
    return retItem;
}

Game JsonConverter::ConvertToGame(const Json& item){
    std::string name;
    int id, minPlayers, maxPlayers, numRounds;
    bool audienceEnabled;
    Game::GameProgress progress = item.at("GameProgress").template get<Game::GameProgress>();
    item.at("GameId").get_to(id);
    Game retGame(id);
    item.at("GameName").get_to(name);
    item.at("MinPlayers").get_to(minPlayers);
    item.at("MaxPlayers").get_to(maxPlayers);
    item.at("NumRounds").get_to(numRounds);
    item.at("AudienceEnabled").get_to(audienceEnabled);
    retGame.SetGameName(name);
    retGame.SetMinPlayers(minPlayers);
    retGame.SetMaxPlayers(maxPlayers);
    retGame.SetAudienceEnabled(audienceEnabled);
    retGame.SetNumRounds(numRounds);
    retGame.SetGameProgress(progress);

    // Set constants from Json array
    auto jsonConstants = item.at("GameConstants");
    std::for_each(jsonConstants.begin(), jsonConstants.end(), [&retGame, this](auto &item){
        GameConstant constant = this->ConvertToGameConstant(item);
        retGame.AddConstant(constant.GetName(), constant);
    });

    // Set variables from Json array
    auto jsonVariables = item.at("GameVariables");
    std::for_each(jsonVariables.begin(), jsonVariables.end(), [&retGame, this](auto &item){
        GameVariable var = this->ConvertToGameVariable(item);
        retGame.AddVariable(var.GetName(), var);
    });

    return retGame;
}
