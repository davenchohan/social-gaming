#include "ExecutionQueue.h"

void ExecutionQueue::push(std::unique_ptr<ExecutionNode> node){
    baseQ.push(std::move(node));
    size++;
}

std::unique_ptr<ExecutionNode> ExecutionQueue::pop(){
    if(!baseQ.empty()){
        size--;
        auto rv = std::move(baseQ.front());
        baseQ.pop();
        return rv;
    }else{
        size = 0;
        return std::unique_ptr<ExecutionNode>(nullptr);
    }
}

bool ExecutionQueue::empty(){
    return baseQ.empty();
}

int ExecutionQueue::getSize(){
    return size;
}

/*
Not Too sure if we actually need this, will not write a unit test for it
*/
void ExecutionQueue::executeAll(){
    while(!baseQ.empty()){
        std::unique_ptr<ExecutionNode> node_ptr = std::move(baseQ.front());
        node_ptr->execute();
        baseQ.pop();
    }
    return;
}