#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Nodes.h"

class ExecutionQueue{
private:
    std::queue<std::unique_ptr<ExecutionNode>> baseQ;
public:
    ExecutionQueue() {}
    void push(std::unique_ptr<ExecutionNode> );
    void pop();
    void executeAll();
    };