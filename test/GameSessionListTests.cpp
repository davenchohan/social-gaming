#include "gtest/gtest.h"
#include "GameSessionList.h"
#include "GameSessionHandler.h"
#include <stdexcept>

TEST(GameSessionListTest, AddGameSessionHandler) {
    GameSessionList sessionList;

    // Create a sample game session handler
    Game gameObj(1);
	Player host ("Host", 0);
    GameSessionHandler gameSessionHandler(123, gameObj, host);

    // Add the game session handler to the list
    sessionList.AddGameSessionHandler("session1", gameSessionHandler);

    // Ensure that the session exists in the list
    EXPECT_TRUE(sessionList.DoesSessionExist("session1"));
}

TEST(GameSessionListTest, GetGameSessionHandlers) {
    GameSessionList sessionList;

    Game gameObj(1);
	Player host ("Host", 0);

    Game gameObj2(1);
	Player player ("player", 1);
    // Create sample game session handlers
    GameSessionHandler gameSessionHandler1(1, gameObj, host);
    GameSessionHandler gameSessionHandler2(2, gameObj2, player);

    // Add the game session handlers to the list
    sessionList.AddGameSessionHandler("session1", gameSessionHandler1);
    sessionList.AddGameSessionHandler("session2", gameSessionHandler2);

    // Get the list of game session handlers
    auto handlers = sessionList.GetGameSessionHandlers();

    // Ensure that the list contains both session handlers
    EXPECT_EQ(handlers.size(), 2);
    EXPECT_TRUE(handlers.find("session1") != handlers.end());
    EXPECT_TRUE(handlers.find("session2") != handlers.end());
}

TEST(GameSessionListTest, GetGameSessionHandler) {
    GameSessionList sessionList;

    // Create a sample game session handler
    Game gameObj(1);
	Player player ("player", 1);
    GameSessionHandler gameSessionHandler(1, gameObj, player);

    // Add the game session handler to the list
    sessionList.AddGameSessionHandler("session1", gameSessionHandler);

    // Get the specific game session handler
    GameSessionHandler retrievedHandler = sessionList.GetGameSessionHandler("session1");

    // Ensure that the retrieved handler is the same as the added handler
    EXPECT_EQ(retrievedHandler, gameSessionHandler);
}
