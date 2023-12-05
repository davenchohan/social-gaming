#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Nodes.h"

class ExecutionQueue{
private:
    std::queue<std::unique_ptr<ExecutionNode>> baseQ;
    int size;
public:
    ExecutionQueue() : size(0) {}
    void push(std::unique_ptr<ExecutionNode>);
    std::unique_ptr<ExecutionNode> pop();
    bool empty();
    int getSize();
    // No unit test for executeAll
    void executeAll();
    ~ExecutionQueue() = default;
};