#include "gtest/gtest.h"
#include "Player.h"

TEST(PlayerTest, PlayerStateInitialization) {
    // Create a player with a specific name and ID
    Player player("Joe", 123);

    // Ensure that the player's state is initialized correctly
    EXPECT_EQ(player.GetPlayerState(), Player::WaitingTurn);
}

TEST(PlayerTest, SetPlayerState) {
    // Create a player
    Player player("Bob", 456);

    // Set the player's state to Playing
    player.SetPlayerState(Player::Active);

    // Ensure that the player's state is updated correctly
    EXPECT_EQ(player.GetPlayerState(), Player::Active);
}

TEST(PlayerTest, UserInheritance) {
    // Create a player with a specific name and ID
    Player player("Charlie", 789);

    // Ensure that the player inherits the name and ID from the User class
    EXPECT_EQ(player.GetName(), "Charlie");
    EXPECT_EQ(player.GetUserId(), 789);
}
