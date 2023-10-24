#include "ParserLibrary.h"

Parser::Parser(std::string key, std::string item){
    subject[key] = item;
}

std::string Parser::subjectToString(){
    return subject.dump();
}

RequestParser::RequestParser(std::string req){
    request.append("Request", req);
}

std::string RequestParser::ConstructRequest(){
    return request.subjectToString();
}



