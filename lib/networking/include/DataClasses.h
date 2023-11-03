#pragma once
#include <string>

class DataClass {
public:
    virtual std::string toJSON() = 0;
    virtual ~DataClass() = default;
};

class JoinGame : public DataClass {
private:
    std::string userID;
    std::string gameID;

public:
    JoinGame(const std::string& userID, const std::string& gameID);
    std::string toJSON() override;
};

class CreateGame : public DataClass {
private:
    std::string gameID;
    std::string config;
    std::string userID;

public:
    CreateGame(const std::string& userID, const std::string& gameID, const std::string& config);
    std::string toJSON() override;
};

class GetGame : public DataClass {
private:
    std::string gameID;

public:
    GetGame(const std::string& gameID);
    std::string toJSON() override;
};