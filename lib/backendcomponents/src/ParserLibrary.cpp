#include "ParserLibrary.h"

Parser::Parser(std::string key, std::string item){
    subject[key] = item;
}

void Parser::append(std::string key, std::string item){
    subject[key] = item;
}

std::string Parser::subjectToString(){
    return subject.dump();
}

RequestParser::RequestParser(std::string req){
    request.append("Request", req);
}

void RequestParser::appendRequest(const std::string key, const std::string val){
    request.append(key, val);
}

std::string RequestParser::ConstructRequest(){
    return request.subjectToString();
}



