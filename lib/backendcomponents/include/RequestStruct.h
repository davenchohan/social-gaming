#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

struct RequestInfo{
    std::string request;
    std::string gameName;
    std::string gameID;
    json gameConfig;
    json players;
};
