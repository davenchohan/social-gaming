#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ParserLibrary.h"
#include <string>
#include <map>
#include <vector>

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
    std::string expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestParserClassAppendBasic){
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    RequestConstructor parser("ReqCreateGame");
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    std::string expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
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
    std::string expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
    std::string generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserClassInitialize){
    RequestConstructor parser("SomeRequest");
    auto expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"SomeRequest\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserGetValue){
    Json someJson;
    someJson["Prompt"] = "This is a prompt";
    RequestParser parser;
    auto expected = "This is a prompt";
    auto generated = parser.getValue("Prompt", someJson);
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequetParserGetNonExisitentValue){
    Json someJson;
    RequestParser parser;
    auto expected = "NO_Value_Found";
    auto generated = parser.getValue("nonexistent", someJson);
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserGetValue2){
    RequestParser parser("{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"SomeGame\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}");
    auto expected = "SomeGame";
    auto generated = parser.getValue("GameName");
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserGetNullValue){
    RequestParser parser("{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"SomeGame\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}");
    auto expected = "NO_Value_Found";
    auto generated = parser.getValue("nonexistent");
    EXPECT_EQ(expected, generated);
}

TEST(ParserLibraryTests, TestRequestParserAppendBasic){
    RequestConstructor parser("ReqCreateGame");
    std::map<std::string, std::string> values = { {"Name","Gabe"}, {"GameName","Rock, Paper, Scissors"}};
    for (auto const&[key, val] : values){
        parser.appendItem(key, val);
    }
    std::string expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
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
    auto expected = "{\"ConnID\":\"\",\"GameConfig\":{\"Rules\":[\"Move\",\"Cannot stand still\",\"Must Jump every 2 turns\"]},\"GameID\":\"\",\"GameName\":\"Rock, Paper, Scissors\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
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
    std::string expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"Chess\",\"Name\":\"Gabe\",\"Players\":{},\"Request\":\"ReqCreateGame\",\"misc\":null}";
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
    std::vector<Player> players = {Player{"Gabe", 1234}, Player{"Peter", 1212}};
    req.players = players;
    Json blankJson;
    req.gameConfig = blankJson;
    req.misc = blankJson;
    RequestConstructor constructor(req);

    auto generated = constructor.ConstructRequest();
    auto expected = "{\"ConnID\":\"\",\"GameConfig\":null,\"GameID\":\"1111\",\"GameName\":\"Chess\",\"Players\":[{\"id\":1234,\"name\":\"Gabe\",\"playerState\":\"WaitingTurn\"},{\"id\":1212,\"name\":\"Peter\",\"playerState\":\"WaitingTurn\"}],\"Request\":\"ReqGetGame\",\"misc\":null}";
    EXPECT_EQ(generated, expected);
}

TEST(ParserLibraryTests, TestRequestParserAddPlayers){
    std::vector<Player> players = {Player{"Gabe", 1234}, Player{"Peter", 1212}};
    std::for_each(players.begin(), players.end(), [](auto &item){
        item.SetPlayerState(Player::PlayerState::Active);
    });
    RequestConstructor con;
    con.appendItem("Players", players);
    auto generated = con.ConstructRequest();
    auto expected = "{\"Players\":[{\"id\":1234,\"name\":\"Gabe\",\"playerState\":\"Active\"},{\"id\":1212,\"name\":\"Peter\",\"playerState\":\"Active\"}]}";
    EXPECT_EQ(generated, expected);
}

TEST(ParserLibraryTests, TestRequestParserAddMembers){
    std::vector<AudienceMember> members = { AudienceMember{"Gabe", 1234}, AudienceMember{"Peter",1111}};
    std::for_each(members.begin(), members.end(), [](auto &item){
        item.SetAudienceState(AudienceMember::AudienceMemberState::Active);
    });
    RequestConstructor con;
    con.appendItem("AudienceMembers", members);
    auto generated = con.ConstructRequest();
    auto expected = "{\"AudienceMembers\":[{\"audienceMemberState\":\"Active\",\"id\":1234,\"name\":\"Gabe\"},{\"audienceMemberState\":\"Active\",\"id\":1111,\"name\":\"Peter\"}]}";
    EXPECT_EQ(generated, expected);
}

TEST(ParserLibraryTests, TestMapToVec){
    std::map<std::string, int>testMap{ {"item1",1}, {"item2",2}, {"item3",3} };
    std::vector<int> dumpVector;
    std::vector<int> expectedVector = {1,2,3};

    MapToVec(testMap, dumpVector);
    EXPECT_EQ(dumpVector, expectedVector);
}

TEST(ParserLibraryTests, TestConvertToGame){
    Game expectedGame(1234);
    expectedGame.SetGameName("Chess");
    expectedGame.SetMinPlayers(2);
    expectedGame.SetMaxPlayers(2);
    expectedGame.SetAudienceEnabled(true);
    expectedGame.SetNumRounds(1);
    expectedGame.SetGameProgress(Game::GameProgress::NotStarted);

    Json toParse;
    toParse["GameName"] =  "Chess";
    toParse["GameId"] = 1234;
    toParse["MinPlayers"] = 2;
    toParse["MaxPlayers"] = 2;
    toParse["AudienceEnabled"] = true;
    toParse["NumRounds"] = 1;
    toParse["GameProgress"] = Game::GameProgress::NotStarted;
    toParse["GameConstants"] = nullptr;
    toParse["GameVariables"] = nullptr;

    JsonConverter converter;
    auto generated = converter.ConvertToGame(toParse);
    EXPECT_EQ(generated, expectedGame);
}

TEST(ParserLibraryTests, TestConvertToGameFilled){
    std::vector<GameConstant> constants = {{"constant1", "Queen"}, {"constant2", "King"}};
    std::vector<GameVariable> variables = {{"variable1", 1234,"timer"}};

    JsonConverter converter;
    Game expectedGame(1234);
    expectedGame.SetGameName("Chess");
    expectedGame.SetMinPlayers(2);
    expectedGame.SetMaxPlayers(2);
    expectedGame.SetAudienceEnabled(true);
    expectedGame.SetNumRounds(1);
    expectedGame.SetGameProgress(Game::GameProgress::NotStarted);
    expectedGame.AddConstant("constant1", constants[0]);
    expectedGame.AddConstant("constant2", constants[1]);
    expectedGame.AddVariable("variable1", variables[0]);

    std::vector<Json> jsonConstants;
    std::for_each(constants.begin(), constants.end(), [&jsonConstants, &converter](GameConstant& item){
        jsonConstants.push_back(converter.ConvertFromGameConstant(item));
    });
    std::vector<Json> jsonVariables;
    std::for_each(variables.begin(), variables.end(), [&jsonVariables, &converter](GameVariable &item){
        jsonVariables.push_back(converter.ConvertFromGameVariable(item));
    });

    ASSERT_EQ(jsonConstants.size(), constants.size());
    ASSERT_EQ(jsonVariables.size(), variables.size());

    Json toParse;
    toParse["GameName"] =  "Chess";
    toParse["GameId"] = 1234;
    toParse["MinPlayers"] = 2;
    toParse["MaxPlayers"] = 2;
    toParse["AudienceEnabled"] = true;
    toParse["NumRounds"] = 1;
    toParse["GameProgress"] = Game::GameProgress::NotStarted;
    toParse["GameConstants"] = jsonConstants;
    toParse["GameVariables"] = jsonVariables;

    auto generated = converter.ConvertToGame(toParse);
    EXPECT_EQ(generated, expectedGame);
}

TEST(ParserLibraryTests, TestRequestParserConnId){
    RequestConstructor parser("SomeRequest");
    parser.appendItem("ConnID","123456");
    auto expected = "{\"ConnID\":\"123456\",\"GameConfig\":null,\"GameID\":\"\",\"GameName\":\"\",\"Players\":{},\"Request\":\"SomeRequest\",\"misc\":null}";
    auto generated = parser.ConstructRequest();
    EXPECT_EQ(expected, generated);
}


