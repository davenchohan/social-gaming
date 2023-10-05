#include <string>
#include "CustomExceptions.h"

const std::string UnknownGameException::what(){
    return "Error, Unknown Game: " + std::string(this->gameName);
}

const std::string IncompleteGameException::what(){
    return "Error, Incomplete game spec filled for: " + std::string(this->gameName);
}

const std::string UnknownGameSpecException::what(){
    return "Error, Incomplete game spec filled for: " + std::string(this->gameSpec);
}

const std::string UnknownPlayerException::what(){
    return "Error, player not found: " + std::string(this->playerName);
}

const std::string UnknownRequestException::what(){
    return "Error, invalid request: " + std::string(this->requestStr);
}