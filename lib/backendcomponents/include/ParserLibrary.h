#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>
#include <vector>
#include <map>

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

Json getJsonItem(const std::string);
