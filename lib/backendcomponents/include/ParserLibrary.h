#include <string>
#include "Structures.h"

class ParserLibraryInterface{
public:
    ParserLibraryInterface() = default;
    std::string ConstructRequest(RequestInfo&);
    std::string ConstructResponse(ResponseInfo&);
    Json parseJson(std::string&);
    RequestInfo* parseRequest(std::string&);
};

