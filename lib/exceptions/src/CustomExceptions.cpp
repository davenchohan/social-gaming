#include <string>
#include "CustomExceptions.h"

const std::string UnknownGameException::what(){
    return "Error, Unknown Game: " + this->gameName;
}

const std::string IncompleteGameException::what(){
    return "Error, Incomplete game spec filled for: " + this->gameName;
}

const std::string UnknownGameSpecException::what(){
    return "Error, Unknown game spec: " + this->gameSpec;
}

const std::string UnknownPlayerException::what(){
    return "Error, player not found: " + this->playerName;
}

const std::string UnknownRequestException::what(){
    return "Error, invalid request: " + this->requestStr;
}