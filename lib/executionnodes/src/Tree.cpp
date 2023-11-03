

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "nodes.h"
#include "Tree.h"

ExecutionTree::ExecutionTree(){
    start = nullptr;
    current = nullptr;
    end = nullptr;
};
ExecutionTree::ExecutionTree(ExecutionNode* node){
    start= node;
    current= node;
    end = node;
};

void ExecutionTree::append(ExecutionNode* node){
    if(start!=nullptr){
        end->next = node;
        end = node;
    }
    else{
        start,current,end = node;
    }

};
