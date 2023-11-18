#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <Game.h>
#include <GameVariable.h>
#include <GameConstant.h>

using namespace testing;

// Test cases for the Game class
TEST(GameTest, DefaultConstructor) {
    Game game(1);
    EXPECT_EQ(game.GetGameName(), "Game");
    EXPECT_EQ(game.GetMinPlayers(), 2);
    EXPECT_EQ(game.GetMaxPlayers(), 4);
    EXPECT_TRUE(game.IsAudienceEnabled());
    EXPECT_EQ(game.GetNumRounds(), 3);
    EXPECT_EQ(game.GetGameProgress(), Game::NotStarted);
}

TEST(GameTest, SetGameName) {
    Game game(1);
    game.SetGameName("NewGameName");
    EXPECT_EQ(game.GetGameName(), "NewGameName");
}

TEST(GameTest, SetAndGetMinPlayers) {
    // Test SetMinPlayers and GetMinPlayers methods
    Game game(1);
    game.SetMinPlayers(3);
    EXPECT_EQ(game.GetMinPlayers(), 3);
}

TEST(GameTest, SetAndGetMaxPlayers) {
    // Test SetMaxPlayers and GetMaxPlayers methods
    Game game(1);
    game.SetMaxPlayers(6);
    EXPECT_EQ(game.GetMaxPlayers(), 6);
}

TEST(GameTest, SetAndIsAudienceEnabled) {
    // Test SetAudienceEnabled and IsAudienceEnabled methods
    Game game(1);
    game.SetAudienceEnabled(false);
    EXPECT_FALSE(game.IsAudienceEnabled());
}

TEST(GameTest, SetAndGetNumRounds) {
    // Test SetNumRounds and GetNumRounds methods
    Game game(1);
    game.SetNumRounds(5);
    EXPECT_EQ(game.GetNumRounds(), 5);
}

TEST(GameTest, GetAllVariablesTest) {
    Game game(123);
    game.AddVariable("Value1", GameVariable("Value1", 1, 12));
    game.AddVariable("Value2", GameVariable("Value2", 2, 345));
    game.AddVariable("Value3", GameVariable("Value3", 3, "hello"));
    std::vector<GameVariable> variableValues = game.GetAllVariables();

    EXPECT_EQ(variableValues.size(), 3);

    EXPECT_TRUE(std::find(variableValues.begin(), variableValues.end(), GameVariable("Value1", 1, 12)) != variableValues.end());
    EXPECT_TRUE(std::find(variableValues.begin(), variableValues.end(), GameVariable("Value2", 2, 345)) != variableValues.end());
    EXPECT_TRUE(std::find(variableValues.begin(), variableValues.end(), GameVariable("Value3", 3, "hello")) != variableValues.end());
}

TEST(GameTest, GetAllConstantsTest) {
    Game game(123);
    game.AddConstant("Constant1", GameConstant("Constant1", "hello"));
    game.AddConstant("Constant2", GameConstant("Constant2", "world"));
    game.AddConstant("Constant3", GameConstant("Constant3", "123"));
    std::vector<GameConstant> constantValues = game.GetAllConstants();

    EXPECT_EQ(constantValues.size(), 3);

    EXPECT_TRUE(std::find(constantValues.begin(), constantValues.end(), GameConstant("Constant1", "hello")) != constantValues.end());
    EXPECT_TRUE(std::find(constantValues.begin(), constantValues.end(), GameConstant("Constant2", "world")) != constantValues.end());
    EXPECT_TRUE(std::find(constantValues.begin(), constantValues.end(), GameConstant("Constant3", "123")) != constantValues.end());
}

TEST(GameTest, AddAndGetConstant) {
    Game game(123);
    game.AddConstant("Constant1", GameConstant("Constant1", "hello"));

    EXPECT_EQ(game.GetConstant("Constant1"), GameConstant("Constant1", "hello"));
}

TEST(GameTest, AddAndGetVariablle) {
    Game game(123);
    game.AddVariable("Value1", GameVariable("Value1", 1, 123456));

    EXPECT_EQ(game.GetVariable("Value1"), GameVariable("Value1", 1, 123456));
}

