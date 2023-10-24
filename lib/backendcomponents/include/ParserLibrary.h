#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <string>
#include <vector>

class Parser{
private:
    Json subject;
public:
    Parser(){}
    Parser(std::string, std::string);
    void append(const std::string, const std::string);
    std::string subjectToString();
};


class RequestParser{
public:
    RequestParser(std::string);
    void appendRequest(const std::string, const std::string);
    std::string ConstructRequest();
private:
    Parser request;
};
