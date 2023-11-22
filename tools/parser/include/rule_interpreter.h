#pragma once
#include <cpp-tree-sitter.h>
#include "Game.h"
#include "Player.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <cassert>
#include <cstdio>
#include <memory>
#include "ExpressNodes.h"
#include "Nodes.h"


const std::vector<std::string> humanInputTypes = {
    "message",
    "input_choice",
  };

const std::vector<std::string> controlTypes = {
    "for",
    "while",
    "parallel_for",
    "match",
    "inparallel"
};
const std::vector<std::string> listTypes = {
    "extend",
    "reverse",
    "shuffle",
    "sort",
    "discard"
};
//enum converters for all possible node types
//some still need to be added 
const std::map<std::string, ControlTypes> controlToValue={
    {"for", ControlTypes::FOR},
    {"while",ControlTypes::WHILE},
    {"parallel_for", ControlTypes::PARALLEL_FOR},
    {"match", ControlTypes::MATCH},
    {"inparallel", ControlTypes::INPARALLEL}
};
const std::map<std::string, Builtins> builtinToValue={
    {"size", Builtins::SIZE},
    {"upfrom",Builtins::UPFROM},
    {"collect", Builtins::COLLECT},
    {"contains", Builtins::CONTAINS}
 
};
const std::map<std::string, InputTypes> inputToValue={
    {"text", InputTypes::TEXT},
    {"choice", InputTypes::CHOICE},
    {"range",InputTypes::RANGE},
    {"vote", InputTypes::VOTE}
};
const std::map<std::string, InteractionType> interactionEnum={
    {"message", InteractionType::MESSAGE},
    {"input_choice", InteractionType::INPUTCHOICE},
   
};
 const std::map<std::string, ListTypes> listToValue = {
    {"extend", ListTypes::EXTEND},
    {"reverse", ListTypes::REVERSE},
    {"shuffle", ListTypes::SHUFFLE},
    {"sort", ListTypes::SORT},
    {"discard", ListTypes::DISCARD}
};
const std::map<std::string, ExpressionTypes> expToValue = {
    {"!=", ExpressionTypes::BOOLEAN},
    {"=", ExpressionTypes::BOOLEAN},
    {"=>", ExpressionTypes::BOOLEAN},
    {"=<", ExpressionTypes::BOOLEAN},
    {">", ExpressionTypes::BOOLEAN},
    {"<", ExpressionTypes::BOOLEAN},
    {"||", ExpressionTypes::BOOLEAN},
    {"&&", ExpressionTypes::BOOLEAN},
    {"+", ExpressionTypes::OPERATION},
    {"-", ExpressionTypes::OPERATION},
    {"*", ExpressionTypes::OPERATION},
    {"/", ExpressionTypes::OPERATION},
    {"builtin",ExpressionTypes::BUILTIN},
    {"indentifier",ExpressionTypes::IDENTIFIER},


};
const std::map<std::string,SimpleType>  simpleToEnum = {
    {"boolean",SimpleType::BOOLEAN},
    {"identifier",SimpleType::IDENTIFIER},
    {"quoted_string",SimpleType::STRING},
    {"number",SimpleType::NUMBER}
};
const std::map<std::string,OpType>  opToEnum = {
    {"+",OpType::ADD},
    {"-",OpType::SUB},
    {"*",OpType::MULT},
    {"/",OpType::DIV},

};
const std::map<std::string,CompType> compTypeToEnum = {
    {"!=", CompType::NOTEQ},
    {"=", CompType::EQ},
    {"=>", CompType::LESSEQ},
    {"=<", CompType::GREATEREQ},
    {">", CompType::LESS},
    {"<", CompType::GREATER},
    {"||", CompType::OR},
    {"&&", CompType::AND}
};




ListTypes getlistToValue(std::string_view type);
InputTypes getInputTypeValue(std::string_view type);
ControlTypes getControlTypeValue(std::string_view type);

bool isControlType(std::string type);

void forLoopHandler(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void whileLoopHanler(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void parallel_forHandler(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void handleMatch(ActiveGame&active, ExecutionTree& tree, ts::Node node);
void inparallelHandler(ActiveGame& active, ExecutionTree& tree, ts::Node node);

ExecutionNode* parseMatchEntry(ActiveGame&active, ExecutionTree& tree, ts::Node node);

void handleControlType(ActiveGame& active, ExecutionTree& tree, ts::Node node);

void handleText();
void handleChoice();
void handleRange();
void handleVote();
void handleInputChoice(ActiveGame &active, ExecutionTree &tree, ts::Node node) ;
bool isHumanInput(std::string type);
void handleInput(ActiveGame& active, ExecutionTree& tree, ts::Node node);



bool isListType(std::string type);
void handleListOperation(ActiveGame& active, ExecutionTree& tree, ts::Node node);

bool isTiming(std::string type);
void handleTiming(ActiveGame& active, ExecutionTree& tree, ts::Node node);

void ruleVisitor(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void bodyHandler(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void rulesHandler(ActiveGame& active, ExecutionTree& tree, ts::Node node);
void recursiveIdent(ActiveGame &active,std::vector<std::string>& identifiers, ts::Node node);
void handleMessageRule(ActiveGame &active, ExecutionTree &tree, ts::Node node); 



ExpressionNode* parseExpression(ActiveGame &active, ExecutionTree& tree,ts::Node node);
ExpressionNode* parseBuiltIn(ActiveGame &active, ExecutionTree& tree,ts::Node node);
void recursiveIdent(ActiveGame &active,std::vector<std::string>& identifiers, ts::Node node);
ExpressionNode* parseComparison(ActiveGame &active, ExecutionTree& tree,ts::Node node);
ExpressionNode* parseForExpression(ActiveGame &active, ExecutionTree& tree,ts::Node node);
ExpressionNode* parseSimpleExpression(ActiveGame &active, ExecutionTree& tree,ts::Node node);
ExpressionNode* parseOperatorExpression(ActiveGame &active, ExecutionTree& tree,ts::Node node);
ExpressionNode* parseBuiltIn(ActiveGame &active, ExecutionTree& tree,ts::Node node);