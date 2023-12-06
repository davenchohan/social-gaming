#include "AudienceMember.h"

// Constructor
AudienceMember::AudienceMember(const std::string& name, int userId) : User(name, userId), audienceState(Inactive) {

}

// Set audience member state
void AudienceMember::SetAudienceState(const AudienceMemberState& state) {
    audienceState = state;
}

// Get audience member state
AudienceMember::AudienceMemberState AudienceMember::GetAudienceState() const {
    return audienceState;
}