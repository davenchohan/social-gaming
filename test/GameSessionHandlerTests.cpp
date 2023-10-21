#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameSessionHandler.h"
#include <string>
#include <stdexcept>

TEST(GameSessionHandlerTests, AddAndGetPlayer) {
	Game gameObj;
	GameSessionHandler ghandler(1, gameObj);

	Player p1 ("Player1", 1);
	ghandler.AddPlayer("Player1", p1);

	Player p1 ("Player2", 2);
	ghandler.AddPlayer("Player2", p1);

	EXPECT_EQ(player2, ghandler.GetPlayer("Player2"));
	EXPECT_EQ(player1, ghandler.GetPlayer("Player1"));
	// tests nonexistent player
	try {
		Player p3 = ghandler.GetPlayer("Player3");
	} catch (const char* msg) {
		EXPECT_EQ("Player not found", msg);
	}
}

TEST(GameSessionHandlerTests, RemovePlayer) {
	Game gameObj;
	GameSessionHandler ghandler(2, gameObj);

	Player p1 ("Player1", 1);
	ghandler.AddPlayer("Player1", p1);

	Player p1 ("Player2", 2);
	ghandler.AddPlayer("Player2", p1);

	ghandler.RemovePlayer("Player1");
	try {
		Player p3 = ghandler.GetPlayer("Player1");
	} catch (const char* msg) {
		EXPECT_EQ("Player not found", msg);
	}
	ghandler.RemovePlayer("Player2");
	try {
		Player p4 = ghandler.GetPlayer("Player2");
	} catch (const char* msg) {
		EXPECT_EQ("Player not found", msg);
	}
}

TEST(GameSessionHandlerTests, AddandGetAudienceMember) {
	Game gameObj;
	GameSessionHandler ghandler(3, gameObj);

	AudienceMember audience1("Audience1");
	AudienceMember audience1("Audience2");
	ghandler.AddAudienceMember("Audience1", audience1);
	ghandler.AddAudienceMember("Audience2", audience2);

	EXPECT_EQ(audience1, ghandler.GetAudienceMember("Audience1"));
	EXPECT_EQ(audience2, ghandler.GetAudienceMember("Audience2"));
	// tests nonexistent AudienceMember
	try {
		AudienceMember audience3 = ghandler.GetAudienceMember("Audience3");
	} catch (const char* msg) {
		EXPECT_EQ("Audience Member not found", msg);
	}
}

TEST(GameSessionHandlerTests, RemoveAudienceMember) {
	Game gameObj;
	GameSessionHandler ghandler(4, gameObj);

	AudienceMember audience1("Audience1");
	ghandler.AddAudienceMember("Audience1", audience1);

	ghandler.RemoveAudienceMember("Audience1");
	EXPECT_THROW(ghandler,GetAudienceMember("Audience1"), std::runtime_error);
}

TEST(GameSessionHandlerTests, SetAndGetCurrentRound) {
	Game gameObj;
	GameSessionHandler ghandler(5, gameObj);
	ghandler.SetCurrentRound(1);
	EXPECT_EQ(1, ghandler.GetCurrentRound());
	ghandler.SetCurrentRound(2);
	EXPECT_EQ(2, ghandler.GetCurrentRound());
}

TEST(GameSessionHandlerTests, GetGame) {
	Game gameObj;
	GameSessionHandler ghandler(6, gameObj);
	EXPECT_EQ(gameObj, ghandler.GetGame());
}

// TEST(GameSessionHandlerTests, StartAndEndSession) {

// }

TEST(GameSessionHandlerTests, GetSessionId) {
	Game gameObj;
	GameSessionHandler ghandler(7, gameObj);
	EXPECT_EQ(7, ghandler.GetSessionId());
}