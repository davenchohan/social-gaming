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