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
    void execute();
    ExecutionNode* next;
    
    virtual ~ExecutionNode() = default;
private:
    virtual void executeImpl() = 0;

};
class ExpressionNode : public ExecutionNode {
public: 
    ExpressionNode();
    bool evaluate();
private:
    void executeImpl();
};


class ForNode : public ExecutionNode {
public:
    ForNode();
    ForNode(ExpressionNode* expression, ExecutionTree* loop);
    ExecutionNode* getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
private:
    void executeImpl();
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
    void executeImpl();
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
    void executeImpl();
    ExpressionNode* condition; 
    ExecutionTree* loop;   

};

class InParallelNode : public ExecutionNode {
public:
    InParallelNode();
private:
    void executeImpl();
     
};    

class MatchNode : public ExecutionNode {
public:
    MatchNode();
private:
    //ListType matchable;
    void executeImpl();
         
};

class ExtendNode : public ExecutionNode {
public:
    ExtendNode();
private:
    void executeImpl();
       
};



class TimerNode : public ExecutionNode {
public:
    TimerNode();
private:
    void executeImpl();
      
};


class InputChoiceNode : public ExecutionNode {
public:
    InputChoiceNode();
private:
    void executeImpl();
      
};


class MessageNode : public ExecutionNode {
public:
    MessageNode();
private:
    void executeImpl();
      
};



class VariableAssignmentNode : public ExecutionNode {
public:
    VariableAssignmentNode();
private:
    void executeImpl();
      
};



class LiteralNode : public ExecutionNode {
public:
    LiteralNode();
private:
    void executeImpl();
      
};

