#include "gtest/gtest.h"
#include "User.h"

TEST(UserTest, GetName) {
    // Create a user with a specific name
    User user("Alice", 123);

    // Ensure that GetName returns the correct name
    EXPECT_EQ(user.GetName(), "Alice");
}

TEST(UserTest, GetUserId) {
    // Create a user with a specific ID
    User user("Bob", 456);

    // Ensure that GetUserId returns the correct ID
    EXPECT_EQ(user.GetUserId(), 456);
}

TEST(UserTest, EqualityComparison) {
    // Create two users with the same ID
    User user1("Joe", 123);
    User user2("Bob", 123);

    // Ensure that the equality operator works as expected
    EXPECT_TRUE(user1 == user2);
}

TEST(UserTest, InequalityComparison) {
    // Create two users with different IDs
    User user1("Joe", 123);
    User user2("Bob", 456);

    // Ensure that the equality operator works as expected
    EXPECT_FALSE(user1 == user2);
}

TEST(UserTest, LessThanComparison) {
    // Create two users with different IDs
    User user1("Joe", 123);
    User user2("Bob", 456);

    // Ensure that the less-than operator works as expected
    EXPECT_TRUE(user1 < user2);
}
