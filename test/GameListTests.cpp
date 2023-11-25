#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <Game.h>
#include <GameList.h>

using namespace testing;

class GameListTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    GameList gameList;
    Game sampleGame = Game(123);
};

TEST_F(GameListTest, AddGameAndGetGameList) {
    // Add a game to the list
    sampleGame.SetGameName("Rock,Paper,Scissors");
    gameList.AddGame(sampleGame);

    // Retrieve the game list
    std::vector<Game> retrievedGames = gameList.GetGameList();

    // Verify that the retrieved list contains the added game
    EXPECT_EQ(retrievedGames.size(), 1);
    EXPECT_EQ(retrievedGames[0].GetGameName(), "Rock,Paper,Scissors");
}

// Test for GetGameSpec with an existing game
TEST_F(GameListTest, GetGameSpecExistingGame) {
    // Add a game to the list
    sampleGame.SetGameName("Rock,Paper,Scissors");
    gameList.AddGame(sampleGame);

    // Retrieve the game specification for the added game
    Game retrievedGame = gameList.GetGameSpec("Rock,Paper,Scissors");

    // Verify that the retrieved game matches the added game
    EXPECT_EQ(retrievedGame, sampleGame);
}

// Test for GetGameSpec with a non-existing game
TEST_F(GameListTest, GetGameSpecNonExistingGame) {
    // Retrieve the game specification for a non-existing game
    Game retrievedGame = gameList.GetGameSpec("NonExistingGame");

    // Verify that the retrieved game is a default-constructed game (you may need to adjust this based on your Game class)
    EXPECT_EQ(retrievedGame, Game(000));
}