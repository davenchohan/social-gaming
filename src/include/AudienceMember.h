#ifndef AUDIENCEMEMBER_H
#define AUDIENCEMEMBER_H

#include <string>

class AudienceMember {
public:
    // Constructor
    AudienceMember(const std::string& name);

    // Set a state value
    void SetStateValue(const std::string& value);

    // Get the value of a state
    std::string GetStateValue() const;

private:
    std::string audienceState;
    std::string audienceName;
};

#endif
