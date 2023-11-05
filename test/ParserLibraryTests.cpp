#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include "ParserLibrary.h"

using namespace testing;

// Define mocks

// Implemented Tests
TEST(ParserLibraryTests, TestParserClassInitializeEmpty){
    RequestConstructor parser;
    std::string expected = "null";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassInitializeBasic){
    RequestConstructor parser("ReqCreateGame");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendBasic){
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    RequestConstructor parser("ReqCreateGame");
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendReplace){
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    RequestConstructor parser("ReqCreateGame");
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    parser.appendItem("GameName", "Chess");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserClassInitialize){
    RequestConstructor parser("SomeRequest");
    auto expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"SomeRequest\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserAppendBasic){
    RequestConstructor parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserAppendMultiTypes){
    RequestConstructor parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    Json gameConfig;
    std::vector<std::string> rules = {"Move", "Cannot stand still", "Must Jump every 2 turns"};
    gameConfig["Rules"] = rules;
    parser.appendItem("GameConfig", gameConfig);
    auto expected = "{\"GameConfig\":{\"Rules\":[\"Move\",\"Cannot stand still\",\"Must Jump every 2 turns\"]},\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserReplace){
    RequestConstructor parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    parser.appendItem("GameName", "Chess");
    std::string expected = "{\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}


TEST(ParserLibraryTests, TestRequestParserReqInfoBasic){
    RequestConstructor parser("ReqCreateGame");
    parser.appendItem("GameName","Rock,Paper,Scissors");
    Json blankJson;
    std::vector<Player>pl2;
    RequestInfo expected{"ReqCreateGame", "Rock,Paper,Scissors", "", blankJson, pl2, blankJson};
    RequestInfo generated = parser.returnReqInfo();
    EXPECT_EQ(generated, expected);
}

TEST(ParserLibraryTests, TestRequestParserConstructor){
    RequestInfo req;
    req.gameName = "Chess";
    req.gameID = "1111";
    req.request = "ReqGetGame";
    std::vector<Player> players = {Player{"Gabe", "1234"}, Player{"Peter", "1212"}};
    req.players = players;
    Json blankJson;
    req.gameConfig = blankJson;
    req.misc = blankJson;
    RequestConstructor constructor(req);

    auto generated = constructor.ConstructRequest();
    EXPECT_EQ(generated, "");
}


