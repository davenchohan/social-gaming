#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>
#include <vector>

struct RequestInfo{
    std::string request;
    std::string gameName;
    std::string gameID;
    Json gameConfig;
    Json players;
};

struct ResponseInfo{
    std::string request;
    std::vector<Json> objects;
};
