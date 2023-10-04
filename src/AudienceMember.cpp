#include "AudienceMember.h"

// Constructor
AudienceMember::AudienceMember(const std::string& name) : audienceName(name) {
    
}

// Set a state value
void AudienceMember::SetStateValue(const std::string& value) {
    audienceState = value;
}

// Get the value of a state
std::string AudienceMember::GetStateValue() const {
    return audienceState;
}
