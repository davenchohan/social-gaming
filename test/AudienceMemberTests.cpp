#include "gtest/gtest.h"
#include "AudienceMember.h"

TEST(AudienceMemberTest, AudienceStateInitialization) {
    // Create an audience member with a specific name and ID
    AudienceMember audienceMember("Joe", 123);

    // Ensure that the audience member's state is initialized correctly
    EXPECT_EQ(audienceMember.GetAudienceState(), AudienceMember::Inactive);
}

TEST(AudienceMemberTest, SetAudienceState) {
    // Create an audience member
    AudienceMember audienceMember("Bob", 456);

    // Set the audience member's state to Active
    audienceMember.SetAudienceState(AudienceMember::Active);

    // Ensure that the audience member's state is updated correctly
    EXPECT_EQ(audienceMember.GetAudienceState(), AudienceMember::Active);
}

TEST(AudienceMemberTest, UserInheritance) {
    // Create an audience member with a specific name and ID
    AudienceMember audienceMember("Kyle", 789);

    // Ensure that the audience member inherits the name and ID from the User class
    EXPECT_EQ(audienceMember.GetName(), "Kyle");
    EXPECT_EQ(audienceMember.GetUserId(), 789);
}
