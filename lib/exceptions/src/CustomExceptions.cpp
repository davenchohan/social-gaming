#include <string>
#include "CustomExceptions.h"

const std::string UnknownGameException::what(){
    return "Error, Unknown Game: " + std::string(this->gameName);
}

const std::string IncompleteGameException::what(){
    return "Error, Incomplete game spec filled for: " + std::string(this->gameName);
}