#include <string>
#include <vector>
#include <map>
#include <variant>
#include <iostream>
#include "Nodes.h"
#include "Tree.h"
#include "ExpressNodes.h"


// ExecutionNode class
//______________________________________________________________________________________________________________________________
ExecutionNode::ExecutionNode() {
     // I don't think we need a base node class
}

std::unique_ptr<ExecutionNode> ExecutionNode::execute() {
    //executeImpl();
}







// ForNode class
//______________________________________________________________________________________________________________________________
ForNode::ForNode(std::string identifier, ExpressionNode* expression, ExecutionTree loop)
    : identifier(std::move(identifier)), condition(expression), loop(std::make_unique<ExecutionTree>(std::move(loop))) {
}

std::unique_ptr<ExecutionNode> ForNode::executeImpl() {
    // TODO:
}
void ForNode::print()   {
    std::cout << "ForNode:" << std::endl;
    loop.get()->print();
    // Print specific members of ForNode here
}






// WhileNode class
//______________________________________________________________________________________________________________________________
WhileNode::WhileNode(ExpressionNode* condition, ExecutionTree loop )
    : condition(condition),  loop(std::make_unique<ExecutionTree>(std::move(loop))){
        
   
}

std::unique_ptr<ExecutionNode> WhileNode::executeImpl() {
    // TODO: 
}
void WhileNode::print()   {
    std::cout << "WhileNode:" << std::endl;
    // Print specific members of ForNode here
}








// ParallelForNode class
//______________________________________________________________________________________________________________________________
ParallelForNode::ParallelForNode(std::string identifier,ExpressionNode* expression,   ExecutionTree loop)
    :identifier(identifier), condition(expression), loop(std::make_unique<ExecutionTree>(std::move(loop))) {
      
    // Constructor implementation for ParallelForNode
}

std::unique_ptr<ExecutionNode> ParallelForNode::executeImpl() {
    // TODO: Implement execution logic for ParallelForNode
}
void ParallelForNode::print()   {
    std::cout << "ParallelForNode:" << std::endl;
    loop.get()->print();
    // Print specific members of ForNode here
}






// MatchNode class
//______________________________________________________________________________________________________________________________

MatchNode::MatchNode(ExpressionNode* condition ,  std::vector<std::unique_ptr<ExecutionNode>> entries_): condition(condition){
    entries = std::move(entries_);    // Constructor implementation for MatchNode
}


std::unique_ptr<ExecutionNode> MatchNode::executeImpl() {
    // TODO: Implement execution logic for MatchNode
}
void MatchNode::print()   {
    std::cout << "MatchNode:" << std::endl;
    
    // Print specific members of ForNode here
}



//MatchEntryNode
//____________________________________________________________________________________________________________________________
MatchEntryNode::MatchEntryNode(ExpressionNode* entry,  std::unique_ptr<ExecutionTree>  subtree_):entry(entry){
    subtree = std::move(subtree_);
}
void MatchEntryNode::print()   {
    std::cout << "MatchNode:" << std::endl;
    // Print specific members of ForNode here
}

std::unique_ptr<ExecutionNode> MatchEntryNode::executeImpl() {
    // TODO: Implement execution logic for VariableAssignmentNode
}



// MessageNode class
//______________________________________________________________________________________________________________________________
MessageNode::MessageNode(const std::string& message, const std::string& playerSet): message(message), playerSet(playerSet) {
    // Constructor implementation for MessageNode
}

std::unique_ptr<ExecutionNode> MessageNode::executeImpl() {
    // TODO: Implement execution logic for MessageNode
}
void MessageNode::print()   {
    std::cout << "MessageNode:" << std::endl;
    // Print specific members of ForNode here
}






//InputChoiceNode
//_______________________________________________________________________________________________________________________________

InputChoiceNode::InputChoiceNode(std::vector<std::string> recipientIdentifiers,const std::string& prompt, const std::vector<std::string> choicesIdentifiers, const std::vector<std::string> targetIdentifiers, ExpressionNode* timeout)
:recipientIdentifiers(recipientIdentifiers),prompt(prompt),choicesIdentifiers(choicesIdentifiers),targetIdentifiers(targetIdentifiers),timeout(timeout){

                    }
std::unique_ptr<ExecutionNode> InputChoiceNode::executeImpl() {
    // TODO: Implement execution logic for MessageNode
}
void InputChoiceNode::print()   {
    std::cout << "InputChoiceNode:" << std::endl;
    // Print specific members of ForNode here
}





// VariableAssignmentNode class
//______________________________________________________________________________________________________________________________
VariableAssignmentNode::VariableAssignmentNode(std::vector<std::string> identifiers, ExpressionNode* express):express(express),identifiers(identifiers){

    // Constructor implementation for VariableAssignmentNode
}

std::unique_ptr<ExecutionNode> VariableAssignmentNode::executeImpl() {
    // TODO: Implement execution logic for VariableAssignmentNode
}
void VariableAssignmentNode::print()   {
    std::cout << "VariableAssignmentNode:" << std::endl;
    // Print specific members of ForNode here
}








//list OperationNodes
//______________________________________________________________________________________________________________________________
ListOperation::ListOperation(ListTypes type, ExpressionNode* expr1, ExpressionNode* expr2):expr1(expr1),expr2(expr2), type(type){

}
std::unique_ptr<ExecutionNode> ListOperation::executeImpl() {
    // TODO: Implement execution logic for VariableAssignmentNode
}
void ListOperation::print()   {
    std::cout << "ListOperation:" << std::endl;
    // Print specific members of ForNode here
}





/**
 * this is all stuff that is not needed for rock paper scissors
// TimerNode class

TimerNode::TimerNode() {
    // Constructor implementation for TimerNode
}

std::unique_ptr<ExecutionNode> TimerNode::executeImpl() {
    // TODO: Implement execution logic for TimerNode
}



std::unique_ptr<ExecutionNode> InParallelNode::executeImpl() {
    // TODO: Implement execution logic for InParallelNode
}
std::unique_ptr<ExecutionNode> ExtendNode::executeImpl() {
    // TODO: Implement execution logic for ExtendNode
}



*/