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
    // This is dumb, have to implement template function in .h file
    template <class T1> void append(const std::string key, const T1 val){ subject[key] = val;}
    std::string subjectToString();
};


class RequestParser{
public:
    RequestParser(std::string);
    template <class T1> void appendRequest(const std::string key, const T1 val){request.append(key, val);}
    std::string ConstructRequest();
private:
    Parser request;
};
