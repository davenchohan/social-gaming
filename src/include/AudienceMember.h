#ifndef AUDIENCEMEMBER_H
#define AUDIENCEMEMBER_H

#include "User.h"

class AudienceMember : public User {
public:
    // Constructor
    AudienceMember(const std::string& name, int userId);

};

#endif
