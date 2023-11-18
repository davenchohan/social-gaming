#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameSessionHandler.h"
#include <string>
#include <stdexcept>

TEST(GameSessionHandlerTests, AddAndGetPlayer) {
	Game gameObj(1);
	Player host ("Host", 0);
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
		FAIL() << "Expected GetPlayer error message not found";
	}
}

TEST(GameSessionHandlerTests, RemovePlayer) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	Player p1 ("Player1", 1);
	ghandler.AddPlayer("Player1", p1);

	Player p2 ("Player2", 2);
	ghandler.AddPlayer("Player2", p2);

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

	Player p2 ("Player2", 2);
	ghandler.AddPlayer("Player2", p2);

	ghandler.RemovePlayer("Player2");
	ghandler.RemovePlayer("Player1");

	// tests if player 1 is deleted
	try {
		ghandler.GetPlayer("Player1");
	} 
	catch (std::runtime_error const& err) {
		EXPECT_EQ(err.what(), std::string("Player not found"));
	}
	catch (...) {
		FAIL() << "Expected GetPlayer error message not found";
	}
	// tests if player 2 is deleted
	try {
		ghandler.GetPlayer("Player2");
	} 
	catch (std::runtime_error const& err) {
		EXPECT_EQ(err.what(), std::string("Player not found"));
	}
	catch (...) {
		FAIL() << "Expected GetPlayer error message not found";
	}
}


TEST(GameSessionHandlerTests, AddandGetAudienceMember) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	AudienceMember audience1("Audience1", 1);
	AudienceMember audience2("Audience2", 2);

	ghandler.AddAudienceMember("Audience1", audience1);
	ghandler.AddAudienceMember("Audience2", audience2);

	EXPECT_EQ(audience1, ghandler.GetAudienceMember("Audience1"));
	EXPECT_EQ(audience2, ghandler.GetAudienceMember("Audience2"));
	// tests nonexistent AudienceMember
	try {
		ghandler.GetAudienceMember("Audience3");
	} 
	catch (std::runtime_error const& err) {
		EXPECT_EQ(err.what(), std::string("Audience Member not found"));
	}
	catch (...) {
		FAIL() << "Expected GetAudienceMember error message not found";
	}
}

TEST(GameSessionHandlerTests, RemoveAudienceMember) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	AudienceMember audience1("Audience1", 1);
	AudienceMember audience2("Audience2", 2);
	ghandler.AddAudienceMember("Audience1", audience1);
	ghandler.AddAudienceMember("Audience2", audience2);

	ghandler.RemoveAudienceMember("Audience1");
	// tests if deletion is successful
	try {
		ghandler.GetAudienceMember("Audience1");
	} 
	catch (std::runtime_error const& err) {
		EXPECT_EQ(err.what(), std::string("Audience Member not found"));
	}
	catch (...) {
		FAIL() << "Audience1 deletion failed";
	}
}

TEST(GameSessionHandlerTests, SetAndGetCurrentRound) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	ghandler.SetCurrentRound(1);
	EXPECT_EQ(1, ghandler.GetCurrentRound());
	ghandler.SetCurrentRound(2);
	ghandler.SetCurrentRound(3);
	EXPECT_EQ(3, ghandler.GetCurrentRound());
}

TEST(GameSessionHandlerTests, GetGame) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	EXPECT_EQ(gameObj, ghandler.GetGame());
}


TEST(GameSessionHandlerTests, GetSessionId) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	EXPECT_EQ(1, ghandler.GetSessionId());
}

TEST(GameSessionHandlerTests, SetAndGetHost) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	Player host1 ("Host1",1);
	ghandler.SetHost(host1);

	EXPECT_EQ(host1, ghandler.GetHost());
}

TEST(GameSessionHandlerTests, EqualityOperator) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler1(1, gameObj, host);

	Game gameObj2(2);
	Player host2 ("Host2", 2);
	GameSessionHandler ghandler2(1, gameObj2, host2);
	
	bool compare = false;
	if (ghandler1 == ghandler2) {
		compare = true;
	}
	EXPECT_EQ(true, compare);
}

TEST(GameSessionHandlerTests, LessThanOperator) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler1(2, gameObj, host);

	Game gameObj2(2);
	Player host2 ("Host2", 2);
	GameSessionHandler ghandler2(1, gameObj2, host2);
	
	bool compare = false;
	if (ghandler2 < ghandler1) {
		compare = true;
	}
	EXPECT_EQ(true, compare);
}

TEST(GameSessionHandlerTests, SetAndGetRoomCode) {
	Game gameObj(1);
	Player host ("Host", 0);
	GameSessionHandler ghandler(1, gameObj, host);

	ghandler.SetRoomCode("room code 001");
	EXPECT_EQ("room code 001", ghandler.GetRoomCode());
	ghandler.SetRoomCode("room code 002");
	EXPECT_EQ("room code 002", ghandler.GetRoomCode());

}