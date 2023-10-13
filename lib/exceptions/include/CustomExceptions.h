
#include <exception>
#include <string>

class UnknownGameException : public std::exception{
    private:
        std::string gameName;
    public: 
    UnknownGameException(std::string name) : gameName(name) {}
    const std::string what();
};

class IncompleteGameException : public std::exception{
    private:
        std::string gameName;
    public:
    IncompleteGameException(std::string name) : gameName(name) {}
    const std::string what();
};

class UnknownGameSpecException : public std::exception{
    private:
        std::string gameSpec;
    public:
    UnknownGameSpecException(std::string spec) : gameSpec(spec) {}
    const std::string what();
};

class UnknownPlayerException : public std::exception{
    private:
        std::string playerName;
    public:
    UnknownPlayerException(std::string name) : playerName(name) {}
    const std::string what();
};

class UnknownRequestException : public std::exception{
    private:
        std::string requestStr;
    public:
    UnknownRequestException(std::string request) : requestStr(request) {}
    const std::string what();
};