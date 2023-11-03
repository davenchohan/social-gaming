#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>
#include <vector>
#include <map>

#include "User.h"
#include "Player.h"
#include "AudienceMember.h"
#include "GameConstant.h"
#include "Game.h"

struct RequestInfo{
    std::string request;
    std::string gameName;
    std::string gameID;
    Json gameConfig;
    std::map<std::string, int> players;
    Json misc;
    // Needs to support holding audience members


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

class RequestConstructor{
private:
    Json subject;
public:
    RequestConstructor(){}
    RequestConstructor(std::string);
    // This is dumb, have to implement template function in .h file
    template <class T1> void appendItem(const std::string key, const T1 val){ subject[key] = val;}
    std::string ConstructRequest();
    RequestInfo returnReqInfo();
};


class RequestParser{
    private:
        Json subject;
    public:
        RequestParser(std::string&);
        RequestInfo getRequestStruct();
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


class JsonConverter{
public:
    JsonConverter()=default;
    Json ConvertFromPlayer(Player&);
    Json GetJsonItem(const std::string);
    Json ConvertFromUser(User&);
    Json ConvertFromAudienceMember(AudienceMember &);

    User ConvertToUser(const Json &);
    Player ConvertToPlayer(const Json&);
    AudienceMember ConvertToAudienceMember(const Json&);
    GameVariable ConvertToGameVariable(const Json&);
};
