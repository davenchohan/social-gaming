#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameSessionHandler.h"
#include <string>
#include <stdexcept>

TEST(GameSessionHandlerTests, AddAndGetPlayer) {
	Game gameObj(1);
	Player host ("Host",0);
	GameSessionHandler ghandler(1, gameObj, host);

	Player p1 ("Player1", 1);
	ghandler.AddPlayer("Player1", p1);

	Player p2 ("Player2", 2);
	ghandler.AddPlayer("Player2", p2);

	EXPECT_EQ(p2, ghandler.GetPlayer("Player2"));
	EXPECT_EQ(p1, ghandler.GetPlayer("Player1"));
	// tests nonexistent player error message
	try {
		ghandler.GetPlayer("Player3");
		//std::runtime_error("Player not found")
	} 
	catch (std::runtime_error const& err) {
		EXPECT_EQ(err.what(), std::string("Player not found"));
	}
	catch (...) {
		FAIL() << "Expected error message not found";
	}
}
