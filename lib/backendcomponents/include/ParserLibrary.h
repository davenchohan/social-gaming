#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include "User.h"
#include "Player.h"
#include "AudienceMember.h"
#include "GameConstant.h"
#include "Game.h"


// Helper function to convert general Map to vector (discards keys)
template <class M, class V>
void MapToVec(const std::map<M,V>& map, std::vector<V>& vector){
    for (const auto& item : map){
        vector.push_back(item.second);
    }
}


struct RequestInfo{
    std::string request;
    std::string gameName;
    std::string gameID;
    Json gameConfig;
    std::vector<Player> players;
    // std::vector<AudienceMember> audienceMembers;
    Json misc;

    // Custom < operator definition, allows struct to be used in sets/maps
    bool operator<(const RequestInfo& a)const
    {
        return (request < a.request && gameName < a.gameName && gameID < a.gameID && gameConfig < a.gameConfig && players < a.players && misc < a.misc);
    }

    //Custom equality comparison operator definition
    bool operator==(const RequestInfo& a) const
    {
        return (request == a.request && gameName == a.gameName && gameID == a.gameID && gameConfig == a.gameConfig && players == a.players && misc == a.misc);
    }
};

NLOHMANN_JSON_SERIALIZE_ENUM( Player::PlayerState, {
    {Player::Active, "Active"},
    {Player::Eliminated, "Eliminated"},
    {Player::WaitingTurn, "WaitingTurn"},
})

NLOHMANN_JSON_SERIALIZE_ENUM( AudienceMember::AudienceMemberState, {
    {AudienceMember::Active, "Active"},
    {AudienceMember::Inactive, "Inactive"},
})

NLOHMANN_JSON_SERIALIZE_ENUM( Game::GameProgress, {
    {Game::NotStarted, "NotStarted"},
    {Game::InProgress, "InProgress"},
    {Game::Completed, "Completed"},
 })


class JsonConverter{
public:
    JsonConverter()=default;
    Json ConvertFromPlayer(Player);
    Json GetJsonItem(const std::string);
    Json ConvertFromUser(User&);
    Json ConvertFromAudienceMember(AudienceMember &);
    Json ConvertFromGameVariable(GameVariable&);
    User ConvertToUser(const Json &);
    Player ConvertToPlayer(const Json&);
    AudienceMember ConvertToAudienceMember(const Json&);
    GameVariable ConvertToGameVariable(const Json&);
    Json ConvertFromGame(const Game&);
    Game ConvertToGame(const Json&);
    GameConstant ConvertToGameConstant(const Json&);
    Json ConvertFromGameConstant(const GameConstant&);



    virtual void convertJsonToPlayersArr(Json&, std::vector<Player>&);
};

class RequestConstructor{
private:
    Json subject;
    JsonConverter converter;
public:
    RequestConstructor(){}
    RequestConstructor(RequestInfo& );
    RequestConstructor(std::string);
    // This is dumb, have to implement template function in .h file
    template <class T1> void appendItem(const std::string key, const T1 val){ subject[key] = val;}
    // Override appendItem function for custom behaviour when appending a list of players into request
    void appendItem(const std::string , const std::vector<Player>);
    void appendItem(const std::string key, const std::vector<AudienceMember> members);
    std::string ConstructRequest();
    RequestInfo returnReqInfo();
};


class RequestParser{
    private:
        Json subject;
        JsonConverter converter;
    public:
        RequestParser(){}
        RequestParser(const std::string&);
        RequestInfo getRequestStruct();
        std::string getValue(const std::string &);
        std::string getValue(const std::string &, Json &);
};

