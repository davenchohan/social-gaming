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

extern "C" {
TSLanguage *tree_sitter_socialgaming();
}
void treePrinter(const std::string& prefix,ts::Node parent){
    if (!parent.isNull()){
        auto numNodes = parent.getNumChildren();
        //std::cout <<"num children" << numNodes << "\n";
        int i = 0;
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

//enum for all possible node types
//some still need to be added 



//map from type string to decision making value

DecisionTypes getCaseValue(std::string_view type){
    std::map<std::string, DecisionTypes> stringToValue;


    //control types 
    stringToValue["for"] = DecisionTypes::FOR;
    stringToValue["while"] = DecisionTypes::WHILE;
    stringToValue["parallel_for"] = DecisionTypes::PARALLEL_FOR;
    stringToValue["match"] = DecisionTypes::MATCH;
    stringToValue["inparallel"] = DecisionTypes::INPARALLEL;


    //input types 
    stringToValue["text"] = DecisionTypes::TEXT;
    stringToValue["choice"] = DecisionTypes::CHOICE;
    stringToValue["range"] = DecisionTypes::RANGE;
    stringToValue["vote"] = DecisionTypes::VOTE;
    
    //list types 
    stringToValue["extend"] = DecisionTypes::EXTEND;
    stringToValue["reverse"] = DecisionTypes::REVERSE;
    stringToValue["shuffle"] = DecisionTypes::SHUFFLE;
    stringToValue["sort"] = DecisionTypes::SORT;
    stringToValue["discard"] = DecisionTypes::DISCARD;
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
std::string getForLoopVal(){
    while(true){};
}

void forLoopHandler(Game &active, ExecutionTree& tree,ts::Node node){
    std::string identifier = (std::string)node.getFieldNameForChild(1);
    getForLoopVal();
    ExpressionNode condition;
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
    switch(getCaseValue(node.getType())){
        case DecisionTypes::FOR :
            forLoopHandler(active,node);
            break;
        case DecisionTypes::WHILE:
            whileLoopHanler(active,node);
            break;
        case DecisionTypes::PARALLEL_FOR:
            parallel_forHandler(active,node);
            break;
        case DecisionTypes::MATCH:
            matchHandler(active,node);
            break;
        case DecisionTypes::INPARALLEL:
            inparallelHandler(active,node);
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
    switch(getCaseValue(node.getType())){
        case DecisionTypes::TEXT : //text
            handleText();
            break;
        case DecisionTypes::CHOICE: //choice
            handleChoice();
            break;
        case DecisionTypes::RANGE: //range
            handleRange();
            break;
        case DecisionTypes::VOTE: //vote
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
    switch(getCaseValue(node.getType())){
        case DecisionTypes::EXTEND: 
            handleExtend();//extend
            break;
        case DecisionTypes::REVERSE: //reverse 
            handleReverse();
            break;
        case DecisionTypes::SHUFFLE: //shuffle
            handleShuffle();
            break;
        case DecisionTypes::SORT: //sort 
            handleSort();
            break;
        case DecisionTypes::DISCARD: //discard
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

void handleTiming(Game& active, ts::Node node){
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
       handleControlType(active,current);
    }
    else if(isListType(type)){
        handleListOperation(active,current);
    }else if(isTiming(type)){
        handleTiming(active,current);
    }else if(isHumanInput(type)){
        handleInput(active,current);
    }else {
        std::cout<<"not recognized rule node\n";
    }
}
void bodyHandler(Game& active, ExecutionTree& tree ,ts::Node node ){
    auto numRules = node.getNumChildren();
    for(int i = 1; i < numRules-1;i++){
        std::cout<< i << " child of "<< numRules<<"" << node.getType()<<"\n";
        ruleVisitor(active ,node.getChild(i));
    }
    
}
void rulesHandler(Game& active, ExecutionTree& tree ,ts::Node node ){
    if(node.getType() != "rules" || node.getNumChildren() > 2){
        std::cout<<"error non rules node in rules handler";
    }
    auto body = node.getChild(1);
    bodyHandler(active, body);
    
}

/**
int main() {
    Player mockPlayer = {"test",1};
    Game mockGame = {1,mockPlayer};
    SGParser p("./lib/gameSpecs/rock_paper_scissors.txt");
    auto tree = p.getRoot();
    auto rules = p.getRules();
    std::cout << tree.getType()<<"\n";
    rulesHandler(mockGame,rules);
    treePrinter("",rules);
    

    //std::cout << p.configToJson().dump();
}
*/