#include "ResponseQueue.h"

std::string ResponseQueue::pop(){
    if (baseQ.empty()){
        return "";
    }else{
        std::string toReturn = baseQ.front();
        baseQ.pop();
        return toReturn;
    }
}

void ResponseQueue::push(const std::string &item){
    baseQ.push(item);
}

std::vector<std::string> ResponseQueue::getAllMessages(){
    std::vector<std::string> items;
    std::queue<std::string> copy = baseQ;
    while(!copy.empty()){
        items.push_back(copy.front());
        copy.pop();
    }
    return items;
}

std::string ResponseQueue::Front(){
    if(!baseQ.empty()){
        return baseQ.front();
    }else{
        return "";
    }
}

