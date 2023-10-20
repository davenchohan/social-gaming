#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>

#include "RequestStruct.h"

namespace ParserLib{
    std::string ConstructRequest(RequestInfo);
    std::string ConstructResponse(RequestInfo);
    RequestInfo* parseJsonObjects(Json);
}