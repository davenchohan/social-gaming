#include <string>
#include <vector>
#include <map>
#include <variant>
#include "Nodes.h"
#include "Tree.h"
#include "ExpressNodes.h"
#include <iostream>
ExpressionNode::ExpressionNode(){
    // contains a boolean expression
}
void ExpressionNode::printIdents(){
    std::cout<<"expression Node\n";
    std::cout<<identifiers.size()<<"\n";
    for(auto ident : identifiers){
        std::cout<<ident<<"\n";

    }
}
OpExpressionNode::OpExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , OpType operation): lhs(lhs), rhs(rhs), operation(operation){

}
void OpExpressionNode::print(){
    std::cout<<"________Op__________\n";
    if(lhs){lhs->print();}
    else{std::cout<<"lhs missing\n";};
    
    switch (operation)
    {
    case OpType::ADD:
        std::cout<<"ADD\n";
        break;
    case OpType::SUB:
        std::cout<<"SUB\n";
        break;
    case OpType::MULT:
        std::cout<<"MULT\n";
        break;
    case OpType::DIV:
        std::cout<<"DIV\n";
        break;
    
    default:
        break;
    }
    if(rhs){rhs->print();}
    else{std::cout<<"Rhs missing\n";};
    std::cout<<"_________END__________\n\n";

}
SimpleExpression::SimpleExpression(SimpleType type, std::string value): type(type), value(value){

}
void SimpleExpression::print(){
    std::cout<<"______Simp______\n";
    switch (type)
    {
    case SimpleType::STRING:
        std::cout<<"string\n";
        break;
    case SimpleType::NUMBER:
        std::cout<<"num\n";
        break;
    case SimpleType::BOOLEAN:
        std::cout<<"bool\n";
        break;
    case SimpleType::IDENTIFIER:
        std::cout<<"ident\n";
        break;
    
    default:
        break;
    }
    std::cout<<value<<"\n";
    std::cout<<"______ENDSimp______\n\n";


}
CompExpressionNode::CompExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , CompType comparison): lhs(lhs),rhs(rhs), comparison(comparison){

}
void CompExpressionNode::print(){
    std::cout<<"_________Comp___________\n";
    if(lhs){lhs->print();}
    else{std::cout<<"lhs missing\n";};
    
    switch (comparison)
    {
    case CompType::EQ:
        std::cout<<"eq\n";
        break;
    case CompType::NOTEQ:
        std::cout<<"noteq\n";
        break;
    case CompType::GREATER:
        std::cout<<"great\n";
        break;
    case CompType::GREATEREQ:
        std::cout<<"greatEq\n";
        break;
    case CompType::LESS:
        std::cout<<"less\n";
        break;
    case CompType::LESSEQ:
        std::cout<<"lessEq\n";
        break;
    case CompType::AND:
        std::cout<<"And\n";
        break;
    case CompType::OR:
        std::cout<<"or\n";
        break;
    default:
        break;
    }
    if(rhs){rhs->print();}
    else{std::cout<<"Rhs missing\n";};
    std::cout<<"_________ENDComp__________\n\n";


}
IdentNode::IdentNode(std::vector<std::string> identifiers): identifiers(std::move(identifiers)){}
void IdentNode::print(){
    std::cout<<"_______Identifiers_________\n";
    for(auto ident : identifiers){
        
        std::cout<<ident<<"\n";
    }
     std::cout<<"__________ENDIdent______________\n\n";;
}

BuiltInNode::BuiltInNode(Builtins type,std::vector<std::string> identifiers_, const std::vector<ExpressionNode*>& args_): builtinType(type),  identifiers(std::move(identifiers_)) ,args(std::move(args_)){
    std::cout<<"num args = "<<args.size()<<"\n";
   // std::cout<<"expression Node\n";
    for(auto ident : identifiers){
        std::cout<<ident<<"\n";

    }
}
void BuiltInNode::print(){
    std::cout<<"_____________builtin_____________\nidentifiers:\n";

    for(auto ident : identifiers){
        std::cout<<ident<<"\n";

    }

    std::cout<<"\nType:\n";
    switch (builtinType){
    case Builtins::UPFROM:
        std::cout<<"upfrom\n";
        break;
    case Builtins::COLLECT:
        std::cout<<"collect\n";
        break;
    case Builtins::CONTAINS:
        std::cout<<"contain\n";
        break;
    case Builtins::SIZE:
        std::cout<<"size\n";
        break;
    }
    std::cout<<"num args = "<<args.size()<<"\n";
    for(auto expr : args){
        expr->print();
    }

   std::cout<<"_____________ENDBuilt______________\n\n";;


}
NotNode::NotNode(ExpressionNode* express): express(express){

}
void NotNode::print(){
    std::cout<<"not\n";
    express->print();


}