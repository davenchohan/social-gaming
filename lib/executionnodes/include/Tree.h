
#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include "Nodes.h"
#include "../../datatypes/include/DataTypes.h"
class ExecutionNode;

class ExecutionTree {
public:
    ExecutionTree();
    ExecutionTree(ExecutionNode* start);
    void execute();
    void append(ExecutionNode* node);

private:
    ExecutionNode* start;
    ExecutionNode* current;
    ExecutionNode* end;
};