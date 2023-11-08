#include <random>
#include <string>
#include "RoomCodeGenerator.h"

int RoomCodeGenerator::getRandomNumber(int min, int max) {
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

std::string RoomCodeGenerator::generateRoomCode() {
    const std::string alphanumericCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int codeLength = 6;
    std::string code;
    for (int i = 0; i < codeLength; i++) {
        code += alphanumericCharacters[getRandomNumber(0, alphanumericCharacters.length() - 1)];
    }
    return code;
}
