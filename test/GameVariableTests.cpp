#include "gtest/gtest.h"
#include "GameVariable.h"

TEST(GameVariableTest, IsInt) {
    // Create a game variable with an int value
    GameVariable intVariable("Score", 1, 42);

    // Ensure that IsInt returns true for an int variable
    EXPECT_TRUE(intVariable.IsInt());
    EXPECT_FALSE(intVariable.IsDouble());
    EXPECT_FALSE(intVariable.IsString());
}

TEST(GameVariableTest, IsDouble) {
    // Create a game variable with a double value
    GameVariable doubleVariable("Temperature", 2, 25.5);

    // Ensure that IsDouble returns true for a double variable
    EXPECT_FALSE(doubleVariable.IsInt());
    EXPECT_TRUE(doubleVariable.IsDouble());
    EXPECT_FALSE(doubleVariable.IsString());
}

TEST(GameVariableTest, IsString) {
    // Create a game variable with a string value
    GameVariable stringVariable("Name", 3, "John");

    // Ensure that IsString returns true for a string variable
    EXPECT_FALSE(stringVariable.IsInt());
    EXPECT_FALSE(stringVariable.IsDouble());
    EXPECT_TRUE(stringVariable.IsString());
}

TEST(GameVariableTest, EqualityComparison) {
    // Create two game variables with the same ID
    GameVariable variable1("Score", 1, 42);
    GameVariable variable2("Time", 1, 30.5);

    // Ensure that the equality operator works as expected
    EXPECT_TRUE(variable1 == variable2);
}

TEST(GameVariableTest, InequalityComparison) {
    // Create two game variables with different IDs
    GameVariable variable1("Score", 1, 42);
    GameVariable variable2("Time", 2, 30.5);

    // Ensure that the inequality operator works as expected
    EXPECT_FALSE(variable1 == variable2);
}

TEST(GameVariableTest, LessThanComparison) {
    // Create two game variables with different IDs
    GameVariable variable1("Score", 1, 42);
    GameVariable variable2("Time", 2, 30.5);

    // Ensure that the less-than operator works as expected
    EXPECT_TRUE(variable1 < variable2);
}

TEST(GameVariableTest, GetName) {
    // Create a game variable with a specific name
    GameVariable gameVariable("MaxScore", 1, 100);

    // Ensure that GetName returns the correct name
    EXPECT_EQ(gameVariable.GetName(), "MaxScore");
}
