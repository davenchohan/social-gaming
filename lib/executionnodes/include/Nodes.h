#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include "../../gameclasses/include/Player.h"
#include "Tree.h"
#include "../../datatypes/include/DataTypes.h"
#include "ExpressNodes.h"

class ExecutionTree; 



class ExecutionNode {
public:
    ExecutionNode();
    std::unique_ptr<ExecutionNode> execute();
    std::unique_ptr<ExecutionNode> next;
    virtual void print()=0;
    virtual ~ExecutionNode() = default;
private:
    //virtual std::unique_ptr<ExecutionNode> executeImpl() = 0;

};


//Expression node enums
//________________________________________________________________________________________________________________________________


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





//loop nodes classes
//________________________________________________________________________________________________________________________________
class ForNode : public ExecutionNode {
public:
    ForNode();
    ForNode(std::string identifier ,ExpressionNode* expression, ExecutionTree loop);
    std::unique_ptr<ExecutionNode> getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
    void print(); 
private:
    std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* condition;
    std::string identifier; 
    std::unique_ptr<ExecutionTree> loop;

};

class LoopEndNode : public ExecutionNode {
    public:
    LoopEndNode();
    LoopEndNode(std::unique_ptr<ExecutionNode> parentLoop);
    ExecutionNode * goToParentLoop(std::unique_ptr<ExecutionNode> parentLoop);
    void print(); 
    private:
    ExecutionNode * parentLoop;

};

class WhileNode : public ExecutionNode {
public:
    WhileNode();
    WhileNode(ExpressionNode* condition,  ExecutionTree loop);
    std::unique_ptr<ExecutionNode> getCurrent();
    bool checkCondition(); 
    void print(); 
private:
    std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* condition;
     
    std::unique_ptr<ExecutionTree> loop;     
};

class ParallelForNode : public ExecutionNode {
public:
    ParallelForNode(std::string identifier, ExpressionNode* condition,  ExecutionTree loop );
    std::unique_ptr<ExecutionNode> getCurrent();
    void IncrementCondition();
    bool checkCondition(); 
    void print(); 
private:
    std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* condition;
    std::string identifier; 
    std::unique_ptr<ExecutionTree> loop;  

};
/**
class InParallelNode : public ExecutionNode {
public:
    InParallelNode();
private:
    std::unique_ptr<ExecutionNode> executeImpl();
     
}; 
*/

//Player action nodes below this is all template these will all probably be reworked
//________________________________________________________________________________________________________________________________

class MatchNode : public ExecutionNode {
public:
    MatchNode(ExpressionNode* condition ,  std::vector<std::unique_ptr<ExecutionNode>> entries);
    void print(); 
private:
    std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* condition ;
    std::vector<std::unique_ptr<ExecutionNode>> entries;
         
};
class MatchEntryNode: public ExecutionNode{
    public:
    MatchEntryNode(ExpressionNode* entry, std::unique_ptr<ExecutionTree> subtree);
    void print(); 
    private:
    std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* entry;
    std::unique_ptr<ExecutionTree> subtree;
};

class InputChoiceNode : public ExecutionNode {
public:
    InputChoiceNode(std::vector<std::string> recipientIdentifiers,
                    const std::string& prompt, 
                    const std::vector<std::string> choicesIdentifiers, 
                    const std::vector<std::string> targetIdentifiers, 
                    ExpressionNode* timeout);
    void print(); 
    

private:
    std::vector<std::string> recipientIdentifiers;
    std::string prompt;
    std::vector<std::string> targetIdentifiers;
    std::vector<std::string> choicesIdentifiers;
    ExpressionNode* timeout;
    std::unique_ptr<ExecutionNode> executeImpl();
};


class MessageNode : public ExecutionNode {
public:
    MessageNode(const std::string& message, const std::string& playerSet);
    void print(); 
  

private:
    std::string message; 
    //messages can have identifiers stored within them in the format of {ident} so in execution
    // the strings will need to be searched for brackets and then the values will need to be inserted before the message is sent.
    std::string playerSet;
    std::unique_ptr<ExecutionNode> executeImpl();
};



//List operation Nodes
//__________________________________________________________________________________________________________________-
class ListOperation : public ExecutionNode {
public:
    ListOperation(ListTypes type, ExpressionNode* expr1, ExpressionNode* expr2 );
    void print(); 
private:
std::unique_ptr<ExecutionNode> executeImpl();
    ExpressionNode* expr1;
    ExpressionNode* expr2;
    ListTypes type;
    
};


//Variable Assignment Node
//__________________________________________________________________________________________________________________-

class VariableAssignmentNode : public ExecutionNode {
public:
    VariableAssignmentNode(std::vector<std::string> identifiers, ExpressionNode* express);
    void print(); 
private:
    std::unique_ptr<ExecutionNode> executeImpl();
    std::vector<std::string> identifiers;
    ExpressionNode* express;
      
};


/**
class ExtendNode : public ExecutionNode {
public:
    ExtendNode();
private:
    std::unique_ptr<ExecutionNode> executeImpl();
       
};




class TimerNode : public ExecutionNode {
public:
    TimerNode();
private:
    std::unique_ptr<ExecutionNode> executeImpl();
      
};
*/



/*

class LiteralNode : public ExecutionNode {
public:
    LiteralNode();
private:
    std::unique_ptr<ExecutionNode> executeImpl();
      
};
*/

