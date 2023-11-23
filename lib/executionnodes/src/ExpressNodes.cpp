#include <string>
#include <vector>
#include <map>
#include <variant>
#include "Nodes.h"
#include "Tree.h"
#include "ExpressNodes.h"
ExpressionNode::ExpressionNode(){
    // contains a boolean expression
}
OpExpressionNode::OpExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , OpType operation): lhs(lhs), rhs(rhs), operation(operation){

}
SimpleExpression::SimpleExpression(SimpleType type, std::string value): type(type), value(value){

}
CompExpressionNode::CompExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , CompType comparison): lhs(lhs),rhs(rhs), comparison(comparison){

}
BuiltInNode::BuiltInNode(Builtins type,std::vector<std::string> identifiers, const std::vector<ExpressionNode*>& args): builtinType(type), identifiers(identifiers),args(args){

}
NotNode::NotNode(ExpressionNode* express): express(express){

}