#include "ExecutionQueue.h"

void ExecutionQueue::push(const std::unique_ptr<ExecutionNode> node){
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
        ExecutionNode* node_ptr = baseQ.front();
        node_ptr->execute();
        baseQ.pop();
    }
    return;
}