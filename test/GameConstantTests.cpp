#include "gtest/gtest.h"
#include "GameConstant.h"

TEST(GameConstantTest, GetConstantValue) {
    // Create a game constant with a specific name and value
    GameConstant gameConstant("SpeedLimit", "60");

    // Ensure that GetConstantValue returns the correct value
    EXPECT_EQ(gameConstant.GetConstantValue(), "60");
}

TEST(GameConstantTest, EqualityComparison) {
    // Create two game constants with the same value
    GameConstant constant1("Gravity", "9.8");
    GameConstant constant2("Acceleration", "9.8");

    // Ensure that the equality operator works as expected
    EXPECT_TRUE(constant1 == constant2);
}

TEST(GameConstantTest, InequalityComparison) {
    // Create two game constants with different values
    GameConstant constant1("Temperature", "25");
    GameConstant constant2("Pressure", "1013");

    // Ensure that the equality operator works as expected
    EXPECT_FALSE(constant1 == constant2);
}

TEST(GameConstantTest, LessThanComparison) {
    // Create two game constants with different values
    GameConstant constant1("Voltage", "120");
    GameConstant constant2("Current", "220");

    // Ensure that the less-than operator works as expected
    EXPECT_TRUE(constant1 < constant2);
}

TEST(GameConstantTest, GetName) {
    // Create a game constant with a specific name and value
    GameConstant gameConstant("MaxScore", "100");

    // Ensure that GetName returns the correct name
    EXPECT_EQ(gameConstant.GetName(), "MaxScore");
}
