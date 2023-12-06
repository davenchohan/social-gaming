#include "gtest/gtest.h"
#include "RandomIdGenerator.h"

TEST(RandomIdGeneratorTest, UniqueIdGeneration) {
    RandomIdGenerator generator;

    // Generate two unique IDs
    int id1 = generator.generateUniqueId();
    int id2 = generator.generateUniqueId();

    // Ensure that the generated IDs are not equal
    EXPECT_NE(id1, id2);
}

TEST(RandomIdGeneratorTest, UniqueIdWithinRange) {
    RandomIdGenerator generator;

    // Generate a unique ID
    int id = generator.generateUniqueId();

    // Ensure that the generated ID is within a reasonable range
    EXPECT_LT(id, std::numeric_limits<int>::max());
}

TEST(RandomIdGeneratorTest, MultipleGenerations) {
    RandomIdGenerator generator;

    // Generate multiple unique IDs
    std::vector<int> ids;
    for (int i = 0; i < 100; ++i) {
        ids.push_back(generator.generateUniqueId());
    }

    // Ensure that all generated IDs are unique
    std::set<int> uniqueIds(ids.begin(), ids.end());
    EXPECT_EQ(ids.size(), uniqueIds.size());
}
