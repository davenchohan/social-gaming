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
    //virtual ExecutionNode* executeImpl() = 0;

};


//Expression node enums
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
    LESSEQ,
    AND,
    OR
};
enum class Builtins {
    SIZE,
    UPFROM,
    CONTAINS,
    COLLECT
};
enum class ControlTypes {
    FOR,
    WHILE,
    PARALLEL_FOR,
    MATCH,
    INPARALLEL,
};
enum class ListTypes {
    EXTEND,
    REVERSE,
    SHUFFLE,
    SORT,
    DISCARD,
    
};
enum class InputTypes {
    TEXT,
    CHOICE,
    RANGE,
    VOTE,
    
};
enum class InteractionType {
    MESSAGE,
    INPUTCHOICE,
  
    
};
enum class TimeTypes {
    TIME
};
enum class ExpressionTypes {
    BOOLEAN,
    OPERATION,
    BUILTIN,
    IDENTIFIER,

};
// might make this not be an execution node not 100 % sure if these will need to have execution implenetations or if they will be able to exist on their own

//general expression node type generic expression nodes should not be created. Expression node will implement a evaluate function to allow for expression evaluation
//_________________________________________________________________________________________________________________________________________________________________
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
    ExecutionNode* executeImpl();
    SimpleType type;
    std::string value;

};






//loop nodes classes
//________________________________________________________________________________________________________________________________
class ForNode : public ExecutionNode {
public:
    ForNode();
    ForNode(std::string identifier ,ExpressionNode* expression, ExecutionTree* loop);
    ExecutionNode* getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
private:
    ExecutionNode* executeImpl();
    ExpressionNode* condition;
    std::string identifier; 
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
class ListOperation : public ExecutionNode {
public:
    ListOperation(ListTypes type, ExpressionNode* expr, std::vector<std::string> identifiers );
private:
    ExpressionNode* expression;
    ListTypes type;
    std::vector<std::string> identifiers;
};
class MatchNode : public ExecutionNode {
public:
    MatchNode(ExpressionNode* condition ,  std::vector<ExecutionNode*> entries);
private:
    ExecutionNode* executeImpl();
    ExpressionNode* condition ;
    std::vector<ExecutionNode*> entries;
         
};
class MatchEntryNode: public ExecutionNode{
    public:
    MatchEntryNode(ExpressionNode* entry, ExecutionTree* subtree);
    private:
    ExpressionNode* entry;
    ExecutionTree* subtree;
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
    InputChoiceNode(std::vector<std::string> recipientIdentifiers,
                    const std::string& prompt, 
                    const std::vector<std::string> choicesIdentifiers, 
                    const std::vector<std::string> targetIdentifiers, 
                    ExpressionNode* timeout);
    

private:
    std::vector<std::string> recipientIdentifiers;
    std::string prompt;
    std::vector<std::string> targetIdentifiers;
    std::vector<std::string> choicesIdentifiers;
    ExpressionNode* timeout;
};


class MessageNode : public ExecutionNode {
public:
    MessageNode(const std::string& message, const std::string& playerSet);
  

private:
    std::string message;
    std::string playerSet;
    ExecutionNode* executeImpl();
};



class VariableAssignmentNode : public ExecutionNode {
public:
    VariableAssignmentNode(std::vector<std::string> identifiers, ExpressionNode* express);
private:
    ExecutionNode* executeImpl();
    std::vector<std::string> identifiers;
    ExpressionNode* express;
      
};



class LiteralNode : public ExecutionNode {
public:
    LiteralNode();
private:
    ExecutionNode* executeImpl();
      
};

