#ifndef AUDIENCEMEMBER_H
#define AUDIENCEMEMBER_H

#include "User.h"

class AudienceMember : public User {
public:
    // Enumeration to represent audience member states
    enum AudienceMemberState {
        Active,        //  They are spectating
        Inactive,    // They have stopped spectating
    };

    // Constructor
    AudienceMember(const std::string& name, int userId);

    // Set the audiences member's state
    void SetAudienceState(const AudienceMemberState& state);

    // Get the audiences member's state
    AudienceMemberState GetAudienceState() const;

private:
    AudienceMemberState audienceState;
};

#endif
