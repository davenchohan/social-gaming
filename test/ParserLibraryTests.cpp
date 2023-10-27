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
    Parser parser("Request", "ReqCreateGame");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendBasic){
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    Parser parser("Request", "ReqCreateGame");
    for (auto const&[key, val] : values){
        parser.append(key, val);
    }
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendReplace){
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    Parser parser("Request", "ReqCreateGame");
    for (auto const&[key, val] : values){
        parser.append(key, val);
    }
    parser.append("GameName", "Chess");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.subjectToString();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserClassInitialize){
    RequestParser parser("SomeRequest");
    auto expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"SomeRequest\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserAppendBasic){
    RequestParser parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendRequest(key, val);
    }
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserAppendMultiTypes){
    RequestParser parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendRequest(key, val);
    }
    Json gameConfig;
    std::vector<std::string> rules = {"Move", "Cannot stand still", "Must Jump every 2 turns"};
    gameConfig["Rules"] = rules;
    parser.appendRequest("GameConfig", gameConfig);
    auto expected = "{\"GameConfig\":{\"Rules\":[\"Move\",\"Cannot stand still\",\"Must Jump every 2 turns\"]},\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserReplace){
    RequestParser parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendRequest(key, val);
    }
    parser.appendRequest("GameName", "Chess");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserReqInfoBasic){
    RequestParser parser("ReqCreateGame");
    parser.appendRequest("GameName","Rock,Paper,Scissors");
    Json blankJson;
    std::map<std::string, int> blankPlayers;
    RequestInfo expected{"ReqCreateGame", "Rock,Paper,Scissors", "", blankJson, blankPlayers,blankJson};
    RequestInfo generated = parser.returnReqInfo();
    EXPECT_EQ(generated, expected);
}

