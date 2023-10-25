#include "parser_test.h"

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

extern "C" {
TSLanguage *tree_sitter_socialgaming();
}
const std::string SGParser::rootFields[] = {"configuration", "constants", "variables", "per_player", "per_audience", "rules"};
const std::string SGParser::configFields[] = {"player_range", "has_audience"};
const std::string SGParser::setupFields[] = {"kind", "prompt", "range", "choices", "default"};

ts::Node SGParser::getRoot() {
    return tree->getRootNode();
}
ts::Node SGParser::getConfig() {
    return tree->getRootNode().getNamedChild(0);
}
ts::Node SGParser::getConstants() {
    return tree->getRootNode().getNamedChild(1);
}
ts::Node SGParser::getVar() {
    return tree->getRootNode().getNamedChild(2);
}
ts::Node SGParser::getPerPlayer() {
    return tree->getRootNode().getNamedChild(3);
}
ts::Node SGParser::getPerAudience() {
    return tree->getRootNode().getNamedChild(4);
}
ts::Node SGParser::getRules() {
    return tree->getRootNode().getNamedChild(5);
}

SGParser::SGParser(const std::string filepath) {
    // Create a language and parser.
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::ifstream t(filepath);
    if (!t.is_open()) {
        throw("[ERROR] incorrect filepath");
    }

    std::stringstream buffer;
    buffer << t.rdbuf();
    t.close();
    source = buffer.str();

    tree = new ts::Tree(parser.parseString(buffer.str()));
}

/**
 * @return child content in text if child exists, otherwise empty string
 */
std::string SGParser::getChildStrByField(ts::Node parent, const std::string field) {
    json j;
    ts::Node child = parent.getChildByFieldName(field);
    if (!child.isNull()) {
        ts::Extent<uint32_t> child_extent = child.getByteRange();
        return source.substr(child_extent.start, child_extent.end - child_extent.start);
    } else {
        return "";
    }
}

json SGParser::setupToJson() {
    json ret;
    json j;
    for (uint32_t i = 3; i < getConfig().getNumNamedChildren(); i++) {
        ts::Node setup_rule = getConfig().getNamedChild(i);
        for (auto field : setupFields) {
            if (getChildStrByField(setup_rule, field) != "") {
                j[field] = getChildStrByField(setup_rule, field);
            }
        }
        ret[getChildStrByField(setup_rule, "name")] = j;
    }
    return ret;
}

json SGParser::configToJson() {
    json ret;
    json j;
    for (auto field : configFields) {
        j[field] = getChildStrByField(getConfig(), field);
    }
    j["setup"] = setupToJson();
    ret[getChildStrByField(getConfig(), "name")] = j;
    return ret;
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
int getCaseValue(std::string_view type){
    std::map<std::string, int> stringToValue;


    //control types 
    stringToValue["for"] = 1;
    stringToValue["while"] = 2;
    stringToValue["parallel_for"] = 3;
    stringToValue["match"] = 4;
    stringToValue["inparallel"] = 5;


    //input types 
    stringToValue["text"] = 1;
    stringToValue["choice"] = 2;
    stringToValue["range"] = 3;
    stringToValue["vote"] = 4;
    
    //list types 
    stringToValue["extend"] = 1;
    stringToValue["reverse"] = 2;
    stringToValue["shuffle"] = 3;
    stringToValue["sort"] = 4;
    stringToValue["reverse"] = 5;
    std::string sType = (std::string)type;
    return stringToValue.at(sType);

}



//control types handler 
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
void forLoopHandler(Game &active,ts::Node node){
    std::string identifier = (std::string)node.getFieldNameForChild(1);
    std::cout <<identifier << "\n";
    //todo
}
void whileLoopHanler(Game &active,ts::Node node){
    //todo
}
void parallel_forHandler(Game &active,ts::Node node){
    //todo
}
void matchHandler(Game &active,ts::Node node){
    //todo
}
void inparallelHandler(Game &active,ts::Node node){
    //todo
}



void handleControlType(Game& active, ts::Node node){
    std::cout<<"controlHandle\n";
    int val = getCaseValue(node.getType());
    switch(val){
        case 1 :
            forLoopHandler(active,node);
            break;
        case 2:
            whileLoopHanler(active,node);
            break;
        case 3:
            parallel_forHandler(active,node);
            break;
        case 4:
            matchHandler(active,node);
            break;
        case 5:
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

}
void handleChoice(){

}
void handleRange(){

}
void handleVote(){

}

bool isHumanInput(std::string type){

     return std::find(humanInputTypes.begin(),humanInputTypes.end(),type) != humanInputTypes.end();
}
void handleInput(Game& active, ts::Node node){
    switch(getCaseValue(node.getType())){
        case 1 : //text
            handleText();
            break;
        case 2: //choice
            handleChoice();
            break;
        case 3: //range
            handleRange();
            break;
        case 4: //vote
            handleVote();
            break;
    }
}


//list types handling 

void handleExtend(){

}
void handleReverse(){

}
void handleShuffle(){

}
void handleSort(){

}
void handleDiscard(){

}
std::vector<std::string> listTypes = {
    "extend",
    "reverse",
    "shuffle",
    "sort",
    "discard"
};
bool isListType(std::string type){
     return std::find(listTypes.begin(),listTypes.end(),type) != listTypes.end();
}
void handleListOperation(Game& active, ts::Node node){
    switch(getCaseValue(node.getType())){
        case 1: 
            handleExtend();//extend
            break;
        case 2: //reverse 
            handleReverse();
            break;
        case 3: //shuffle
            handleShuffle();
            break;
        case 4: //sort 
            handleSort();
            break;
        case 5: //discard
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


void ruleVisitor(Game& active, ts::Node node){
    std::cout<<"ruleVisit\n";
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
void rulesHandler(Game& active ,ts::Node node ){
    if(node.getType() != "rules" || node.getNumChildren() > 2){
        std::cout<<"error non rules node in rules handler";
    }
    auto body = node.getChild(1);
    auto numRules = body.getNumChildren();
    for(int i = 1; i < numRules-1;i++){
        std::cout<< i << " child of "<< numRules<<"" << body.getType()<<"\n";
        ruleVisitor(active ,body.getChild(i));
    }
}

int main() {
    Player mockPlayer = {"test",1};
    Game mockGame = {1,mockPlayer};
    SGParser p("./lib/gameSpecs/rock_paper_scissors.txt");
    auto tree = p.getRoot();
    auto rules = p.getRules();
    std::cout << tree.getType()<<"\n";
    //rulesHandler(mockGame,rules);
    treePrinter("",rules);
    

    //std::cout << p.configToJson().dump();
}
