#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include "ParserLibrary.h"

using namespace testing;

// Define mocks

// Implemented Tests
TEST(ParserLibraryTests, TestParserClassInitializeEmpty){
    Parser parser;
    std::string expected = "null";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassInitializeBasic){
    Parser parser("name", "Gabe");
    std::string expected = "{\"name\":\"Gabe\"}";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendBasic){
    std::map<std::string, std::string> values = { {"name","Gabe"}, {"gameName","Rock, Paper, Scissors"}};
    Parser parser;
    for (auto const&[key, val] : values){
        parser.append(key, val);
    }
    std::string expected = "{\"gameName\":\"Rock, Paper, Scissors\",\"name\":\"Gabe\"}";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}