

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "Nodes.h"
#include "Tree.h"
#include <memory>

ExecutionTree::ExecutionTree(){
    start = nullptr;
    current = nullptr;
    end = nullptr;
};
ExecutionTree::ExecutionTree(std::unique_ptr<ExecutionNode> node) {
    start = std::move(node);
    current = start.get();
    end = start.get();
};


void ExecutionTree::append(std::unique_ptr<ExecutionNode> node) {
    if (start != nullptr) {
        end->next = std::move(node); 
        end = end->next.get();       
    } else {
        start = std::move(node);
        current = start.get();
        end = start.get();
    }
};

void ExecutionTree::print(){
    ExecutionNode* curr = start.get();
    while(curr != nullptr){
        curr->print();
        curr = curr->next.get();
    }
}
