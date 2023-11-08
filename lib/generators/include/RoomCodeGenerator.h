#pragma once

#include <random>
#include <string>

class RoomCodeGenerator {
public:
    std::string generateRoomCode();

private:
    int getRandomNumber(int min, int max);
};