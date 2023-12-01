#include "ExecutionQueue.h"

void ExecutionQueue::push(std::unique_ptr<ExecutionNode> node){
    baseQ.push(std::move(node));
}

void ExecutionQueue::pop(){
    if(!baseQ.empty()){
        baseQ.pop();
    }else{
        return;
    }
}

void ExecutionQueue::executeAll(){
    while(!baseQ.empty()){
        std::unique_ptr<ExecutionNode> node_ptr = std::move(baseQ.front());
        node_ptr->execute();
        baseQ.pop();
    }
    return;
}