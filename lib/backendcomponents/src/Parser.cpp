#include "Parser.h"

/*
This function takes in a RequestInfo struct and constructs a string based on the fields within someReq
General Output Format:
{ "reqType" : "REQUEST_NAME",
    "body": [ {"Info1":"DATA_VALUES"}, {"Info2" :"DATA_VALUES"} ]
}
*/

std::string ConstructRequest(RequestInfo someReq){
    return "empty";
}

/*
*/
std::string ConstructResponse(RequestInfo someReq){
    return "empty";
}


Json* parseJson(std::string someString){
    return nullptr;
}

RequestInfo* parseRequest(std::string someRequest){
    return nullptr;
}