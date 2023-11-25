
#pragma once
#include "Nodes.h"
#include "Tree.h"
#include <string>
#include <vector>
#include <map>
#include <variant>

enum class Builtins {
    SIZE,
    UPFROM,
    CONTAINS,
    COLLECT
};
enum class SimpleType {
    BOOLEAN,
    NUMBER,
    STRING,
    IDENTIFIER
};
enum class OpType {
    ADD,
    SUB,
    MULT,
    DIV
};
enum class CompType {
    EQ,
    GREATER,
    LESS,
    NOTEQ,
    GREATEREQ,
    LESSEQ,
    AND,
    OR
};
//general expression node type generic expression nodes should not be created. Expression node will implement a evaluate function to allow for expression evaluation
//_________________________________________________________________________________________________________________________________________________________________
class ExpressionNode {
public: 
    ExpressionNode();
    bool evaluate();
    std::vector<std::string> identifiers;
private:
    bool isFor;
    bool isSimple = false;
    //virtual std::variant<> getValue(); // this will be implemented for all expression node types to allow for node evaluation
};



class BuiltInNode : public ExpressionNode {
public:
    BuiltInNode(Builtins type,std::vector<std::string> identifiers, const std::vector<ExpressionNode*>& args);
private:
    std::vector<std::string> identifiers;
    Builtins builtinType;
    std::vector<ExpressionNode*> args;
};

class OpExpressionNode : public ExpressionNode{
    public: 
    OpExpressionNode();
    OpExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , OpType operation);
    private:
    ExpressionNode* lhs;
    ExpressionNode* rhs;
    OpType operation;
    
};
class NotNode : public ExpressionNode{
    public: 
    NotNode(ExpressionNode* express);
    private:
    ExpressionNode* express;

};

class CompExpressionNode : public ExpressionNode{
    public: 
    CompExpressionNode();
    CompExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , CompType comparison);
    private:
    ExpressionNode* lhs;
    ExpressionNode* rhs;
    CompType comparison;

};




class SimpleExpression : public ExpressionNode{
    public:
    SimpleExpression(SimpleType type, std::string value);
    
    private:
    SimpleType type;
    std::string value;

};


