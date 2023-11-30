#include <queue>
#include <vector>
#include <string>


// FIFO queue with custom methods to support getting values from queue that are not normally supported 
class ResponseQueue{
private:
    std::queue<std::string> baseQ;
public:
    ResponseQueue(){}
    // Response Queue functions
    std::string pop();
    void push(const std::string& );
    std::vector<std::string> getAllMessages();
    std::string Front();
};