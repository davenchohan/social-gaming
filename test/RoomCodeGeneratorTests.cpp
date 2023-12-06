#include "gtest/gtest.h"
#include "RoomCodeGenerator.h"

TEST(RoomCodeGeneratorTest, ValidCodeLength) {
    RoomCodeGenerator codeGenerator;

    // Generate a room code
    std::string code = codeGenerator.generateRoomCode();

    // Ensure that the generated code has the correct length
    EXPECT_EQ(code.length(), 6);
}

TEST(RoomCodeGeneratorTest, CodeContainsAlphanumericCharacters) {
    RoomCodeGenerator codeGenerator;

    // Generate a room code
    std::string code = codeGenerator.generateRoomCode();

    // Ensure that the generated code contains only alphanumeric characters
    for (char c : code) {
        EXPECT_TRUE(std::isalnum(c)) << "Invalid character in room code: " << c;
    }
}

TEST(RoomCodeGeneratorTest, UniqueGeneratedCodes) {
    RoomCodeGenerator codeGenerator;

    // Generate multiple room codes
    const int numCodes = 100;
    std::set<std::string> generatedCodes;
    for (int i = 0; i < numCodes; ++i) {
        generatedCodes.insert(codeGenerator.generateRoomCode());
    }

    // Ensure that all generated codes are unique
    EXPECT_EQ(generatedCodes.size(), numCodes);
}
