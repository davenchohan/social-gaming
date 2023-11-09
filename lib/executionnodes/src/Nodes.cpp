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

void ExecutionNode::execute() {
    executeImpl();
}


// ForNode class

ForNode::ForNode(ExpressionNode* expression,   ExecutionTree* loop)
    : condition(expression), loop(loop) {
     // TODO:
}

void ForNode::executeImpl() {
    // TODO:
}

// WhileNode class

WhileNode::WhileNode(ExpressionNode* condition, ExecutionTree* loop )
    : condition(condition), loop(loop){
    // TODO:
}

void WhileNode::executeImpl() {
    // TODO: 
}



// ParallelForNode class

ParallelForNode::ParallelForNode(ExpressionNode* condition,  ExecutionTree* loop   )
    :  condition(condition), loop(loop) {
    // Constructor implementation for ParallelForNode
}

void ParallelForNode::executeImpl() {
    // TODO: Implement execution logic for ParallelForNode
}


// InParallelNode class

InParallelNode::InParallelNode(){
    // Constructor implementation for InParallelNode
}

void InParallelNode::executeImpl() {
    // TODO: Implement execution logic for InParallelNode
}

ExpressionNode::ExpressionNode(){
    // contains a boolean expression
}

void ExpressionNode::executeImpl(){
    //TODO:
}



// MatchNode class

MatchNode::MatchNode(){
    // Constructor implementation for MatchNode
}

void MatchNode::executeImpl() {
    // TODO: Implement execution logic for MatchNode
}


// ExtendNode class

ExtendNode::ExtendNode(){
    // Constructor implementation for ExtendNode
}

void ExtendNode::executeImpl() {
    // TODO: Implement execution logic for ExtendNode
}



// TimerNode class

TimerNode::TimerNode() {
    // Constructor implementation for TimerNode
}

void TimerNode::executeImpl() {
    // TODO: Implement execution logic for TimerNode
}


// InputChoiceNode class

InputChoiceNode::InputChoiceNode(){
    // Constructor implementation for InputChoiceNode
}

void InputChoiceNode::executeImpl() {
    // TODO: Implement execution logic for InputChoiceNode
}



// MessageNode class

MessageNode::MessageNode() {
    // Constructor implementation for MessageNode
}

void MessageNode::executeImpl() {
    // TODO: Implement execution logic for MessageNode
}


// VariableAssignmentNode class

VariableAssignmentNode::VariableAssignmentNode(){
    // Constructor implementation for VariableAssignmentNode
}

void VariableAssignmentNode::executeImpl() {
    // TODO: Implement execution logic for VariableAssignmentNode
}


// LiteralNode class

LiteralNode::LiteralNode() {
    // Constructor implementation for LiteralNode
}

void LiteralNode::executeImpl() {
    // TODO: Implement execution logic for LiteralNode
}

