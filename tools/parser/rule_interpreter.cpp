
#include "parser_test.h"
#include "rule_interpreter.h"

#include <cpp-tree-sitter.h>
#include "Game.h"
#include "Player.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <memory>
#include <sstream>
#include <string>
#include <algorithm>
#include "Nodes.h"
#include "Tree.h"
#include "parser_test.h"

extern "C" {
TSLanguage *tree_sitter_socialgaming();
}

void treePrinter(const std::string& prefix,ts::Node parent){
    if (!parent.isNull()){
        auto numNodes = parent.getNumChildren();
        //std::cout <<"num children" << numNodes << "\n";
        size_t i = 0;
        if(numNodes != 0){
            while( i < numNodes  ){
                std::cout<< prefix;
                std::cout << (i != numNodes-1 ? "├── " : "└── " );
                auto node = parent.getChild(i);
                std::cout << (std::string)node.getType() << " " ;
                std::cout << "\n";
                treePrinter(prefix + (i != numNodes-1 ?  "│   " : "    "), node);
                i++;
                
            }  
        }
    }
}

// get the byte value of the feild and return the relevent string 

std::string getField(ts::Node node,  const std::string source) {
    if (!node.isNull()) {
        ts::Extent<uint32_t> node_extent = node.getByteRange();
        std::string value  = (std::string)source.substr(node_extent.start, node_extent.end - node_extent.start);
        return value;
    } else {
        return "";
    }
}


//string to enum conversion
//________________________________________________________________________________________________________________________________


SimpleType getSimpType(std::string_view type){
    std::string sType = (std::string)type;
    return simpleToEnum.at(sType);
}
OpType getOpType(std::string_view type){
    std::string sType = (std::string)type;
    return opToEnum.at(sType);
}
ExpressionTypes getExpressionTypeValue(std::string_view type){
    std::string sType = (std::string)type;
    std::cout<<sType<<"\n";
    return expToValue.at(sType);
}
ControlTypes getControlTypeValue(std::string_view type){
    std::string sType = (std::string)type;
    return controlToValue.at(sType);
}
Builtins getBuiltinValue(std::string_view type){
    std::string sType = (std::string)type;
    return builtinToValue.at(sType);
}
CompType getCompType(std::string_view type){
    std::string sType = (std::string)type;
    std::cout<<sType<<"\n";
    return compTypeToEnum.at(sType);
}


//map from type string to decision making value

InputTypes getInputTypeValue(std::string_view type){
      std::string sType = (std::string)type;
    return inputToValue.at(sType);
};
InteractionType getInteracionChoiceValue(std::string_view type){
      std::string sType = (std::string)type;
    return interactionEnum.at(sType);
};
ListTypes getlistToValue(std::string_view type){
    std::string sType = (std::string)type;
    return listToValue.at(sType);

};

//control types handling functions


bool isControlType(std::string type){
     return  std::find(controlTypes.begin(),controlTypes.end(),type) != controlTypes.end();
};






//Expression Node parsers:
//________________________________________________________________________________________________________________________________


/**This switch determine what type of expression is being interpreted allowing for specific expression node types to be created
 * based on the enum for expression type it triggers the relevent parsing expression
*/

ExpressionNode* parseExpression(Game &active, ExecutionTree& tree,ts::Node node){
    std::cout<<"expression handle\n";
    if(node.getNumChildren() == 1){
        return parseSimpleExpression(active,tree,node);
    }else{
        auto expressionType = node.getChild(1).getType();
        treePrinter("",node);
        std::cout <<"expression is "<<expressionType<<"\n";
        if(node.getChild(0).getType() == "!"){
            ExpressionNode* express = parseExpression(active, tree, node.getChild(1));
            return new NotNode(express);
        }
        if(expressionType == "."){
            expressionType = node.getChild(2).getType();
        }
        

        switch(getExpressionTypeValue(expressionType)){
            case ExpressionTypes::BOOLEAN:
                std::cout<<"parseComp\n";
                return parseComparison(active, tree,node);
                break;

            case ExpressionTypes::OPERATION:
             std::cout<<"parseOp\n";
                return parseOperatorExpression(active, tree,node);
                break;

            case ExpressionTypes::BUILTIN:
                std::cout<<"parseBuilt\n";
                return parseBuiltIn(active, tree,node);
                break;

            default:
                std::cout<<"unrecognized expression type\n";

        }
    }
} 



//parsers and creates an operator expressionNode 
ExpressionNode* parseOperatorExpression(Game &active, ExecutionTree& tree,ts::Node node){
    std::cout<<"parseOperatorExpression handle\n";
    ExpressionNode* lhs = parseExpression(active,tree,node.getChild(0));
    ExpressionNode* rhs = parseExpression(active,tree,node.getChild(2));
    OpType expression = getOpType(node.getChild(1).getType());
    
    return new OpExpressionNode {lhs,rhs,expression}; 

   
};

// this handles expressions that are either just identifiers, numbers or values
ExpressionNode* parseSimpleExpression(Game &active, ExecutionTree& tree,ts::Node node){
    std::cout<<"parseSimpleExpression handle\n";
    ts::Node child = node.getChild(0);
    std::string value;
    SimpleType sType = getSimpType(child.getType());
    switch(sType){
    case SimpleType::BOOLEAN:
        value = getField(child.getChild(0),active.getSource());
        break;
    default:
        value = getField(child,active.getSource());
        break;
    }
    
    return new SimpleExpression {sType,value};

}

// This handles expression nodes that use the builtin or in syntax creating a loop identifier as well as a set of values to 
// iterate through for the For loop
ExpressionNode* parseBuiltIn(Game &active, ExecutionTree& tree,ts::Node node){
    std::cout<<"parseBuiltIn handle\n";
    std::vector<std::string> identifiers;
    recursiveIdent(active,identifiers, node);
    ts::Node builtNode = node.getChildByFieldName("builtin");
    
    ts::Node argListNode = node.getChildByFieldName("argument_list"); 
    std::vector<ExpressionNode*> args;
    if (!argListNode.isNull()) {
        auto numArgs = argListNode.getNumChildren();
        for(size_t i = 1; i < numArgs-1;i++){
        std::cout<< i << " child of "<< numArgs<<"" << node.getType()<<"\n";
        auto argNode = argListNode.getChild(i);
        if (argNode.getType() == "expression") {
                ExpressionNode* expr = parseExpression(active,tree,argNode); // Assuming a function to parse expressions
                args.push_back(expr);
            }
        }
    }
    Builtins type = getBuiltinValue(builtNode.getChild(0).getType());
    return new BuiltInNode(type, identifiers, args );
    
}


// This will parse the node into a comparison expression node that holds two expressions and a comparison operation.
ExpressionNode* parseComparison(Game &active, ExecutionTree& tree,ts::Node node){
    
    std::cout<<"parseComp\n";
    ExpressionNode* lhs = parseExpression(active,tree,node.getChild(0));
    ExpressionNode* rhs = parseExpression(active,tree,node.getChild(2));
    CompType expression = getCompType(node.getChild(1).getType());
    CompExpressionNode newNode{lhs,rhs,expression}; 

}

//helper to recursivly traverse nodes and get all identifiers 
void recursiveIdent(Game &active,std::vector<std::string>& identifiers, ts::Node node){
    std::cout<<node.getType()<<"\n";
    if((std::string)node.getType() == "expression"||(std::string)node.getType() == "qualified_identifier" ){
        for(size_t i = 0; i < node.getNumChildren(); i++){
            recursiveIdent(active, identifiers, node.getChild(i));
        }        
    }
    else if((std::string) node.getType() == "identifier"){
        identifiers.emplace_back(getField(node,active.getSource()));
        std::cout<<getField(node,active.getSource())<<"\n";
    }
    else {
        return;
    }
}







//Loop node handlers 
//________________________________________________________________________________________________________________________________


void forLoopHandler(Game &active, ExecutionTree& tree,ts::Node node){
    std::string identifier = getField(node.getChild(1),active.getSource());
    ExpressionNode* condition = parseExpression(active,tree, node.getChild(3));
    ExecutionTree loopTree;
    std::cout <<"identifier "<< identifier << "\n";
    
    //this should collect the body of the loop and create a new tree that is attached to the for loop node.
    ts::Node body = node.getChildByFieldName("body");
    bodyHandler(active,loopTree,body);

    ForNode newNode{identifier,condition, &loopTree};
    //the loop node is then appended to its parent tree.
    //tree.append(&newNode);
   
}

// uneccisary for RPS
void whileLoopHanler(Game &active, ExecutionTree& tree,ts::Node node){
    std::string variable = (std::string)node.getFieldNameForChild(1);
    ExpressionNode* condition = parseExpression(active,tree,node.getChildByFieldName("expression"));
    ExecutionTree loopTree;
    std::cout <<"identifier "<< variable << "\n";
    
    //this should collect the body of the loop and create a new tree that is attached to the for loop node.
    ts::Node body = node.getChildByFieldName("body");
    bodyHandler(active,loopTree,body);

    WhileNode newNode{condition, &loopTree};
    //the loop node is then appended to its parent tree.
    //tree.append(&newNode);
    //todo
}


void parallel_forHandler(Game &active, ExecutionTree& tree,ts::Node node){
    std::string identifier = getField(node.getChild(1),active.getSource());
    ExpressionNode* condition = parseExpression(active,tree, node.getChild(3));
    ExecutionTree loopTree;
    std::cout <<"identifier "<< identifier << "\n";
    
    //this should collect the body of the loop and create a new tree that is attached to the for loop node.
    ts::Node body = node.getChildByFieldName("body");
    bodyHandler(active,loopTree,body);

    ForNode newNode{identifier,condition, &loopTree};
    //the loop node is then appended to its parent tree.
    //tree.append(&newNode);
}
void inparallelHandler(Game &active, ExecutionTree& tree,ts::Node node){
    //todo
}




void handleControlType(Game& active, ExecutionTree& tree, ts::Node node){
    std::cout<<"controlHandle\n";
    // val = getCaseValue(node.getType());
    switch(getControlTypeValue(node.getType())){
        case ControlTypes::FOR :
            forLoopHandler(active,tree,node);
            break;
        case ControlTypes::WHILE:
            whileLoopHanler(active,tree,node);
            break;
        case ControlTypes::PARALLEL_FOR:
            parallel_forHandler(active,tree,node);
            break;
        case ControlTypes::MATCH:
            handleMatch(active,tree,node);
            break;
        case ControlTypes::INPARALLEL:
            inparallelHandler(active,tree,node);
            break;
        default :
        std::cout <<"error invalid control Type\n";
    }
}

//message nodes 
//____________________________________________________________________________________________________________
void handleMessageRule(Game &active, ExecutionTree &tree, ts::Node node) {
    std::cout << "parseMessageRule handle\n";

    ts::Node playerSetNode = node.getChildByFieldName("player_set");
    std::string playerSet;
    //get playerset this should be an enum
    if (!playerSetNode.isNull()) {
        playerSet = playerSetNode.getType();
    }

    ts::Node expressionNode = node.getChildByFieldName("expression");
    std::string message;
    //get the message
    if (!expressionNode.isNull() && expressionNode.getChild(0).getType() == "quotedString" ) {
        message = getField(expressionNode.getChild(0),active.getSource());
    }

   //tree.append(new MessageNode(message, playerSet));
}
// rule_interpreter.cpp
void handleInputChoice(Game &active, ExecutionTree &tree, ts::Node node) {
    std::cout << "parseInputChoiceRule handle\n";

    // Parse the recipient (to)
    ts::Node recipientNode = node.getChild(2);
    
    std::vector<std::string> recipientIdentifiers;
   
    if (!recipientNode.isNull()) {
         recursiveIdent(active,recipientIdentifiers,recipientNode); // A function to extract the identifier text
    }

    // Parse the prompt
    ts::Node promptNode = node.getChild(5);
    std::string prompt;
  
    if (!promptNode.isNull()) {
        prompt = getField(promptNode.getChild(0),active.getSource()); // A function to extract the quoted string
    }

    // Parse the choices
    ts::Node choicesNode = node.getChild(7);
    std::vector<std::string> choicesIdentifiers;
 
    if (!recipientNode.isNull()) {
         recursiveIdent(active,choicesIdentifiers,choicesNode); // A function to extract the identifier text
    }

  
    // Parse the target
    ts::Node targetNode = node.getChild(9);

    std::vector<std::string> targetIdentifiers;
    recursiveIdent(active,targetIdentifiers,targetNode);

    // Parse the timeout
    ts::Node timeoutNode = node.getChild(11);
    ExpressionNode* timeout;
    if (!timeoutNode.isNull()) {
        timeout = parseExpression(active,tree,timeoutNode); // A function to extract the number
    }

    //tree.append( new InputChoiceNode(recipientIdentifiers, prompt, choicesIdentifiers, targetIdentifiers, timeout));
}



//This section will probably get reworked i think that these might not need individual handlers for all node types








//human input node handling 

//human input handler 
void handleText(){
    //todo

}
void handleChoice(){
    //todo

}
void handleRange(){
    //todo

}
void handleVote(){
    //todo

}

bool isHumanInput(std::string type){

     return std::find(humanInputTypes.begin(),humanInputTypes.end(),type) != humanInputTypes.end();
}
void handleInput(Game& active, ExecutionTree& tree, ts::Node node){
    switch(getInteracionChoiceValue(node.getType())){
        case InteractionType::MESSAGE : //send message
            handleMessageRule(active,tree,node);
            break;
        case InteractionType::INPUTCHOICE: //get choice
            handleInputChoice(active,tree,node);
            break;  
    }
}


bool isListType(std::string type){
     return std::find(listTypes.begin(),listTypes.end(),type) != listTypes.end();
}
void handleListOperation(Game& active, ExecutionTree& tree, ts::Node node){
    std::cout << "handleListOperation handle\n";
    ListTypes type = getlistToValue(node.getType());
    ts::Node expressionNode = node.getChildByFieldName("expression");
    ExpressionNode* expr = nullptr;
    if (!expressionNode.isNull()) {
        expr = parseExpression(active, tree, expressionNode);  
    }

    ts::Node fromNode = node.getChildByFieldName("qualified_identifier");
    std::string sourceIdentifier;
    if (!fromNode.isNull()) {
        std::vector<std::string> identifiers;
    }
    //tree.append(new ListOperation(type,expr, identifiers));
}

//timing types handling 
const std::vector<std::string>timingTypes = {
    "time"
};

bool isTiming(std::string type){
    return std::find(timingTypes.begin(),timingTypes.end(),type) != timingTypes.end();
}

void handleTiming(Game& active, ExecutionTree& tree, ts::Node node){
    //handle timing 
  
}
bool isAssignment(std::string type ){
    return type == "assignment";
}
bool isMatch(std::string type){
    return type == "match";
}
void handleAssignement(Game& active, ExecutionTree& tree, ts::Node node){
    std::vector<std::string> identifiers;
    std::cout<<"handling assignement\n";
    recursiveIdent(active, identifiers,node.getChild(0));
    ExpressionNode* express = parseExpression(active,tree,node.getChild(0));
    VariableAssignmentNode newNode {identifiers,express};
    
}
ExecutionNode* parseMatchEntry(Game&active, ExecutionTree& tree, ts::Node node) {
    std::cout<<"parseMatchEntry\n";
    ExpressionNode* ident = parseExpression(active , tree , node.getChild(0));
    ExecutionTree subtree;
    bodyHandler(active, subtree,node.getChildByFieldName("body"));
    return new MatchEntryNode{ident,&subtree};

}

void handleMatch(Game&active, ExecutionTree& tree, ts::Node node){
    std::cout<<"handleMatch\n";
    ExpressionNode* condition = parseExpression(active,tree, node.getChild(1));
    std::vector<ExecutionNode*> entries;
    for( auto i = 3; i < node.getNumChildren()-2;i++){
        auto match = parseMatchEntry(active, tree,node.getChild(i));
        entries.push_back(match);

    }
    //tree.append(new MatchNode{condition,entries});
    //create match execution node

}



void ruleVisitor(Game& active, ExecutionTree& tree, ts::Node node){
    std::cout<<"ruleVisit\n";
    std::string rule = (std::string)node.getType();
    assert(rule == "rule");
    std::cout << node.getType() <<"\n";
    ts::Node current = node.getChild(0);
    std::string type = (std::string)current.getType();
    std::cout << type <<"\n";
    if(isControlType(type)){
       handleControlType(active,tree,current);
    }
    else if(isListType(type)){
        handleListOperation(active,tree,current);
    }else if(isTiming(type)){
        handleTiming(active,tree,current);
    }else if(isHumanInput(type)){
      
        handleInput(active,tree,current);
    }else if(isAssignment(type)){
        handleAssignement(active,tree, current);
    }else if (isMatch(type)){
        handleMatch(active,tree, current);
    }
    else {
        std::cout<<"not recognized rule node\n";
    }
}
void bodyHandler(Game& active, ExecutionTree& tree ,ts::Node node ){
    auto numRules = node.getNumChildren();
    std::cout<<"body\n";
    for(size_t i = 1; i < numRules-1;i++){
        std::cout<< i << " child of "<< numRules<<"" << node.getType()<<"\n";
        ruleVisitor(active,tree ,node.getChild(i));
    }
    
}
void rulesHandler(Game& active, ExecutionTree& tree ,ts::Node node ){
    if(node.getType() != "rules" || node.getNumChildren() > 2){
        std::cout<<"error non rules node in rules handler";
    }
    auto body = node.getChild(1);
    bodyHandler(active,tree, body);
    
}


int main() {
    Player mockPlayer = {"test",1};
    Game mockGame {1};
    std::cout << "start"<<"\n";
    SGParser p("./lib/gameSpecs/rock_paper_scissors.txt");
    mockGame.setSource(p.source);
    auto tree = p.getRoot();
    auto rules = p.getRules();
    treePrinter("",rules);
    std::cout << tree.getType()<<"\n";
    rulesHandler(mockGame,mockGame.gameloop,rules);
   // treePrinter("",rules);
    

    //std::cout << p.configToJson().dump();
}
