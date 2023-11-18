#include <string>
#include <vector>
#include <map>
#include <variant>
#include "Nodes.h"
#include "Tree.h"

// ExecutionNode class

ExecutionNode::ExecutionNode() {
     // I don't think we need a base node class
}

ExecutionNode* ExecutionNode::execute() {
    //executeImpl();
}


// ForNode class

ForNode::ForNode(std::string identifier,ExpressionNode* expression,   ExecutionTree* loop)
    :identifier(identifier), condition(expression), loop(loop) {
     // TODO:
}

ExecutionNode* ForNode::executeImpl() {
    // TODO:
}

// WhileNode class

WhileNode::WhileNode(ExpressionNode* condition, ExecutionTree* loop )
    : condition(condition), loop(loop){
    // TODO:
}

ExecutionNode* WhileNode::executeImpl() {
    // TODO: 
}



// ParallelForNode class

ParallelForNode::ParallelForNode(ExpressionNode* condition,  ExecutionTree* loop   )
    :  condition(condition), loop(loop) {
    // Constructor implementation for ParallelForNode
}

ExecutionNode* ParallelForNode::executeImpl() {
    // TODO: Implement execution logic for ParallelForNode
}

OpExpressionNode::OpExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , OpType operation): lhs(lhs), rhs(rhs), operation(operation){

}
SimpleExpression::SimpleExpression(SimpleType type, std::string value): type(type), value(value){

}
CompExpressionNode::CompExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , CompType comparison): lhs(lhs),rhs(rhs), comparison(comparison){

}
BuiltInNode::BuiltInNode(Builtins type,std::vector<std::string> identifiers, const std::vector<ExpressionNode*>& args): builtinType(type), identifiers(identifiers),args(args){

}


// InParallelNode class

InParallelNode::InParallelNode(){
    // Constructor implementation for InParallelNode
}

ExecutionNode* InParallelNode::executeImpl() {
    // TODO: Implement execution logic for InParallelNode
}

ExpressionNode::ExpressionNode(){
    // contains a boolean expression
}

ExecutionNode* ExpressionNode::executeImpl(){
    //TODO:
}



// MatchNode class

MatchNode::MatchNode(){
    // Constructor implementation for MatchNode
}

ExecutionNode* MatchNode::executeImpl() {
    // TODO: Implement execution logic for MatchNode
}


// ExtendNode class

ExtendNode::ExtendNode(){
    // Constructor implementation for ExtendNode
}

ExecutionNode* ExtendNode::executeImpl() {
    // TODO: Implement execution logic for ExtendNode
}



// TimerNode class

TimerNode::TimerNode() {
    // Constructor implementation for TimerNode
}

ExecutionNode* TimerNode::executeImpl() {
    // TODO: Implement execution logic for TimerNode
}




// MessageNode class

MessageNode::MessageNode(const std::string& message, const std::string& playerSet): message(message), playerSet(playerSet) {
    // Constructor implementation for MessageNode
}
InputChoiceNode::InputChoiceNode(std::vector<std::string> recipientIdentifiers,const std::string& prompt, const std::vector<std::string> choicesIdentifiers, const std::vector<std::string> targetIdentifiers, ExpressionNode* timeout)
:recipientIdentifiers(recipientIdentifiers),prompt(prompt),choicesIdentifiers(choicesIdentifiers),targetIdentifiers(targetIdentifiers),timeout(timeout){

                    }
ExecutionNode* MessageNode::executeImpl() {
    // TODO: Implement execution logic for MessageNode
}


// VariableAssignmentNode class

VariableAssignmentNode::VariableAssignmentNode(){
    // Constructor implementation for VariableAssignmentNode
}

ExecutionNode* VariableAssignmentNode::executeImpl() {
    // TODO: Implement execution logic for VariableAssignmentNode
}


//discardNode

ListOperation::ListOperation(ListTypes type, ExpressionNode* expr, std::vector<std::string> identifiers ):expression(expr), identifiers(identifiers){

}

