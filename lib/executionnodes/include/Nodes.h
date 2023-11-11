#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include "../../gameclasses/include/Player.h"
#include "Tree.h"
#include "../../datatypes/include/DataTypes.h"

class ExecutionTree; 



class ExecutionNode {
public:
    ExecutionNode();
    ExecutionNode* execute();
    ExecutionNode* next;
    
    virtual ~ExecutionNode() = default;
private:
    virtual ExecutionNode* executeImpl() = 0;

};


//Expression node classes
//________________________________________________________________________________________________________________________________
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
    LESSEQ
}
enum class Builtins {
    SIZE,
    UPFROM,
    CONTAINS,
    COLLECT
}
// might make this not be an execution node not 100 % sure if these will need to have execution implenetations or if they will be able to exist on their own

//general expression node type generic expression nodes should not be created. Expression node will implement a evaluate function to allow for expression evaluation
class ExpressionNode : public ExecutionNode {
public: 
    ExpressionNode();
    bool evaluate();
    std::vector<std::string> identifiers;
private:
    bool isFor;
    bool isSimple = false;
    ExecutionNode* executeImpl();
    //virtual std::variant<> getValue(); // this will be implemented for all expression node types to allow for node evaluation
};

class ForExpressionNode : public ExpressionNode{
    public:
    ForExpressionNode(const std::vector<std::string>& identifiers, const ExpressionNode& builtIn);
    void iterate();

};
class BuiltInNode : public ExpressionNode{


}

class OpExpressionNode : public ExpressionNode{
    public: 
    OpExpressionNode(ExpressionNode lhs, ExpressionNode rhs , OpType operation);
    private:
    ExpressionNode lhs;
    ExpressionNode rhs;
    OpType operation;

};

class CompExpressionNode : public ExpressionNode{
    public: 
    OpExpressionNode(ExpressionNode lhs, ExpressionNode rhs , OpType operation);
    private:
    ExpressionNode lhs;
    ExpressionNode rhs;
    CompType comparison;

};




class SimpleExpression : public ExpressionNode{
    public:
    SimpleExpression(SimpleType type, std::string value);
    
    private:
    ExecutionNode* executeImpl();
    SimpleType type;
    std::string value;

};






//loop nodes classes
//________________________________________________________________________________________________________________________________
class ForNode : public ExecutionNode {
public:
    ForNode();
    ForNode(ExpressionNode* expression, ExecutionTree* loop);
    ExecutionNode* getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
private:
    ExecutionNode* executeImpl();
    ExpressionNode* condition;
     
    ExecutionTree* loop;

};

class LoopEndNode : public ExecutionNode {
    public:
    LoopEndNode();
    LoopEndNode(ExecutionNode* parentLoop);
    ExecutionNode * goToParentLoop(ExecutionNode* parentLoop);
    private:
    ExecutionNode * parentLoop;

};

class WhileNode : public ExecutionNode {
public:
    WhileNode();
    WhileNode(ExpressionNode* condition,  ExecutionTree* loop);
    ExecutionNode* getCurrent();
    bool checkCondition(); 
private:
    ExecutionNode* executeImpl();
    ExpressionNode* condition;
     
    ExecutionTree* loop;     
};

class ParallelForNode : public ExecutionNode {
public:
    ParallelForNode(ExpressionNode* condition,  ExecutionTree* loop );
    ExecutionNode* getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
private:
    ExecutionNode* executeImpl();
    ExpressionNode* condition; 
    ExecutionTree* loop;   

};

class InParallelNode : public ExecutionNode {
public:
    InParallelNode();
private:
    ExecutionNode* executeImpl();
     
}; 


//Player action nodes below this is all template these will all probably be reworked
//________________________________________________________________________________________________________________________________

class MatchNode : public ExecutionNode {
public:
    MatchNode();
private:
    //ListType matchable;
    ExecutionNode* executeImpl();
         
};

class ExtendNode : public ExecutionNode {
public:
    ExtendNode();
private:
    ExecutionNode* executeImpl();
       
};



class TimerNode : public ExecutionNode {
public:
    TimerNode();
private:
    ExecutionNode* executeImpl();
      
};


class InputChoiceNode : public ExecutionNode {
public:
    InputChoiceNode();
private:
    ExecutionNode* executeImpl();
      
};


class MessageNode : public ExecutionNode {
public:
    MessageNode();
private:
    ExecutionNode* executeImpl();
      
};



class VariableAssignmentNode : public ExecutionNode {
public:
    VariableAssignmentNode();
private:
    ExecutionNode* executeImpl();
      
};



class LiteralNode : public ExecutionNode {
public:
    LiteralNode();
private:
    ExecutionNode* executeImpl();
      
};

