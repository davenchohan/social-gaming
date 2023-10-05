#include <exception>
#include <string>

class UnknownGameException : public std::exception{
    private:
        char *gameName;
    public: 
    UnknownGameException(char *name) : gameName(name) {}
    const std::string what();
};

class IncompleteGameException : public std::exception{
    private:
        char *gameName;
    public:
    IncompleteGameException(char *name) : gameName(name) {}
    const std::string what();
};

class UnknownGameSpecException : public std::exception{
    private:
        char *gameSpec;
    public:
    UnknownGameSpecException(char *spec) : gameSpec(spec) {}
    const std::string what();
};

class UnknownPlayerException : public std::exception{
    private:
        char *playerName;
    public:
    UnknownPlayerException(char *name) : playerName(name) {}
    const std::string what();
};

class UnknownRequestException : public std::exception{
    private:
        char *requestStr;
    public:
    UnknownRequestException(char *request) : requestStr(request) {}
    const std::string what();
};