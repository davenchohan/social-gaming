#include "ParserLibrary.h"

/*
This function takes in a RequestInfo struct and constructs a string based on the fields within someReq
General Output Format:
{ "reqType" : "REQUEST_NAME",
    "body": [ {"Info1":"DATA_VALUES"}, {"Info2" :"DATA_VALUES"} ]
}
*/
std::string ParserLibraryInterface::ConstructRequest(RequestInfo& someReq){
    return "empty";
}

/*
*/
std::string ParserLibraryInterface::ConstructResponse(ResponseInfo& someResp){
    return "empty";
}


Json ParserLibraryInterface::parseJson(std::string& someString){
    Json temp = Json::parse(someString);
    return temp;
}

RequestInfo* ParserLibraryInterface::parseRequest(std::string& someRequest){
    return nullptr;
}