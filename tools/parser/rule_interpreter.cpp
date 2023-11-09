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

//enum for all possible node types
//some still need to be added 

ControlTypes getControlTypeValue(std::string_view type){
    std::map<std::string, ControlTypes> stringToValue;


    //control types 
    stringToValue["for"] = ControlTypes::FOR;
    stringToValue["while"] = ControlTypes::WHILE;
    stringToValue["parallel_for"] = ControlTypes::PARALLEL_FOR;
    stringToValue["match"] = ControlTypes::MATCH;
    stringToValue["inparallel"] = ControlTypes::INPARALLEL;
    std::string sType = (std::string)type;
    return stringToValue.at(sType);
}


//map from type string to decision making value

InputTypes getInputTypeValue(std::string_view type){
    std::map<std::string, InputTypes> stringToValue;


    //input types 
    stringToValue["text"] = InputTypes::TEXT;
    stringToValue["choice"] = InputTypes::CHOICE;
    stringToValue["range"] = InputTypes::RANGE;
    stringToValue["vote"] = InputTypes::VOTE;
      std::string sType = (std::string)type;
    return stringToValue.at(sType);
}
ListTypes getListTypeValue(std::string_view type){
    std::map<std::string, ListTypes> stringToValue;

    //list types 
    stringToValue["extend"] = ListTypes::EXTEND;
    stringToValue["reverse"] = ListTypes::REVERSE;
    stringToValue["shuffle"] = ListTypes::SHUFFLE;
    stringToValue["sort"] = ListTypes::SORT;
    stringToValue["discard"] = ListTypes::DISCARD;
    std::string sType = (std::string)type;
    return stringToValue.at(sType);

}

//control types handling functions

const std::vector<std::string> controlTypes = {
    "for",
    "while",
    "parallel_for",
    "match",
    "inparallel"
};

bool isControlType(std::string type){
     return  std::find(controlTypes.begin(),controlTypes.end(),type) != controlTypes.end();
}

// create an expression node
ExpressionNode parseExpression(Game &active, ExecutionTree& tree,ts::Node node){
    std::vector<std::string> identifiers;
    std::vector<std::string> arguments;
    if(node.getChild(0).getType() != "!"){
        recursiveIdent(active,identifiers, node);
    }
    ExpressionNode newNode; 
    return newNode;
}
//recursivly traverse nodes and get all identifiers 
void recursiveIdent(Game &active,std::vector<std::string>& identifiers, ts::Node node){
    std::cout<<node.getType()<<"\n";
    if((std::string)node.getType() == "expression"){
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

void forLoopHandler(Game &active, ExecutionTree& tree,ts::Node node){
    std::cout<<"forloop"<<"\n";
    //gets the identifier for the variable in the for loop
    auto identifier = getField(node.getChild(1),active.getSource());
    //getForLoopVal();
    ExpressionNode condition = parseExpression(active,tree, node.getChild(3));
    ExecutionTree loopTree;
    std::cout <<"identifier "<< identifier << "\n";
    
    //this should collect the body of the loop and create a new tree that is attached to the for loop node.
    ts::Node body = node.getChildByFieldName("body");
    bodyHandler(active,loopTree,body);

    ForNode newNode{&condition, &loopTree};
    //the loop node is then appended to its parent tree.
    tree.append(&newNode);
   
}

void whileLoopHanler(Game &active, ExecutionTree& tree,ts::Node node){
    std::string variable = (std::string)node.getFieldNameForChild(1);
    ExpressionNode condition;
    ExecutionTree loopTree;
    std::cout <<"identifier "<< variable << "\n";
    
    //this should collect the body of the loop and create a new tree that is attached to the for loop node.
    ts::Node body = node.getChildByFieldName("body");
    bodyHandler(active,loopTree,body);

    WhileNode newNode{&condition, &loopTree};
    //the loop node is then appended to its parent tree.
    tree.append(&newNode);
    //todo
}

void parallel_forHandler(Game &active, ExecutionTree& tree,ts::Node node){
    //todo
}

void matchHandler(Game &active, ExecutionTree& tree,ts::Node node){
    //todo
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
            matchHandler(active,tree,node);
            break;
        case ControlTypes::INPARALLEL:
            inparallelHandler(active,tree,node);
            break;
        default :
        std::cout <<"error invalid control Type\n";
    }
}





//human input node handling 
std::vector<std::string> humanInputTypes = {
    "text",
    "choice",
    "range",
    "vote"
  };

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
    switch(getInputTypeValue(node.getType())){
        case InputTypes::TEXT : //text
            handleText();
            break;
        case InputTypes::CHOICE: //choice
            handleChoice();
            break;
        case InputTypes::RANGE: //range
            handleRange();
            break;
        case InputTypes::VOTE: //vote
            handleVote();
            break;
    }
}


//list types handling 

void handleExtend(){
    std::cout<<"extend\n";

}
void handleReverse(){
    std::cout<<"revers\n";

}
void handleShuffle(){
    std::cout<<"shuffle\n";

}
void handleSort(){
    std::cout<<"sort\n";

}
void handleDiscard(){
    std::cout<<"discard\n";

}
const std::vector<std::string> listTypes = {
    "extend",
    "reverse",
    "shuffle",
    "sort",
    "discard"
};
bool isListType(std::string type){
     return std::find(listTypes.begin(),listTypes.end(),type) != listTypes.end();
}
void handleListOperation(Game& active, ExecutionTree& tree, ts::Node node){
    switch(getListTypeValue(node.getType())){
        case ListTypes::EXTEND: 
            handleExtend();//extend
            break;
        case ListTypes::REVERSE: //reverse 
            handleReverse();
            break;
        case ListTypes::SHUFFLE: //shuffle
            handleShuffle();
            break;
        case ListTypes::SORT: //sort 
            handleSort();
            break;
        case ListTypes::DISCARD: //discard
            handleDiscard(); 
            break;
    }
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
    }else {
        std::cout<<"not recognized rule node\n";
    }
}
void bodyHandler(Game& active, ExecutionTree& tree ,ts::Node node ){
    auto numRules = node.getNumChildren();
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
    std::cout << tree.getType()<<"\n";
    rulesHandler(mockGame,mockGame.gameloop,rules);
    treePrinter("",rules);
    

    //std::cout << p.configToJson().dump();
}
