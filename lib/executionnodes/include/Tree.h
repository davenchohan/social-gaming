
#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>
#include "Nodes.h"
#include "../../datatypes/include/DataTypes.h"
class ExecutionNode;

class ExecutionTree {
public:
    ExecutionTree();
    ExecutionTree(std::unique_ptr<ExecutionNode> node);
    void execute();
    void append(std::unique_ptr<ExecutionNode> node);
    void print();
    const ExecutionTree deepCopy();

private:
    std::unique_ptr<ExecutionNode> start;
    ExecutionNode* current;
    ExecutionNode* end;
};