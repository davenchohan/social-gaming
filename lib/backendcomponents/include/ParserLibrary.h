#include <string>
#include "Structures.h"

std::string ConstructRequest(RequestInfo);
std::string ConstructResponse(ResponseInfo);
Json* parseJson(std::string);
RequestInfo* parseRequest(std::string);
