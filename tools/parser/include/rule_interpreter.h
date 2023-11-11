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
enum class TimeTypes {
    TIME
};

enum class ExpressionTypes {
    BOOLEAN,
    OPERATION,
    BUILTIN,
    IDENTIFIER,

};
const std::vector<std::string> humanInputTypes = {
    "text",
    "choice",
    "range",
    "vote"
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
//enum for all possible node types
//some still need to be added 
const std::map<std::string, ControlTypes> controlToValue={
    {"for", ControlTypes::FOR},
    {"while",ControlTypes::WHILE},
    {"parallel_for", ControlTypes::PARALLEL_FOR},
    {"match", ControlTypes::MATCH},
    {"inparallel", ControlTypes::INPARALLEL}
};
const std::map<std::string, InputTypes> inputToValue={
    {"text", InputTypes::TEXT},
    {"choice", InputTypes::CHOICE},
    {"range",InputTypes::RANGE},
    {"vote", InputTypes::VOTE}
};
 const std::map<std::string, ListTypes> listToValue = {
    {"extend", ListTypes::EXTEND},
    {"reverse", ListTypes::REVERSE},
    {"shuffle", ListTypes::SHUFFLE},
    {"sort", ListTypes::SORT},
    {"discard", ListTypes::DISCARD}
};
const std::map<std::string, ExpressionTypes> expToValue = {
    {"=>", ExpressionTypes::BOOLEAN},
    {"==", ExpressionTypes::BOOLEAN},
    {"=>", ExpressionTypes::BOOLEAN},
    {"=<", ExpressionTypes::BOOLEAN},
    {">", ExpressionTypes::BOOLEAN},
    {"<", ExpressionTypes::BOOLEAN},
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
    {"/",OpType::DIV}
};




ListTypes getListTypeValue(std::string_view type);
InputTypes getInputTypeValue(std::string_view type);
ControlTypes getControlTypeValue(std::string_view type);

bool isControlType(std::string type);
void forLoopHandler(Game& active, ExecutionTree& tree, ts::Node node);
void whileLoopHanler(Game& active, ExecutionTree& tree, ts::Node node);
void parallel_forHandler(Game& active, ExecutionTree& tree, ts::Node node);
void matchHandler(Game& active, ExecutionTree& tree, ts::Node node);
void inparallelHandler(Game& active, ExecutionTree& tree, ts::Node node);
void handleControlType(Game& active, ExecutionTree& tree, ts::Node node);

void handleText();
void handleChoice();
void handleRange();
void handleVote();
bool isHumanInput(std::string type);
void handleInput(Game& active, ExecutionTree& tree, ts::Node node);

void handleExtend();
void handleReverse();
void handleShuffle();
void handleSort();
void handleDiscard();
bool isListType(std::string type);
void handleListOperation(Game& active, ExecutionTree& tree, ts::Node node);

bool isTiming(std::string type);
void handleTiming(Game& active, ExecutionTree& tree, ts::Node node);

void ruleVisitor(Game& active, ExecutionTree& tree, ts::Node node);
void bodyHandler(Game& active, ExecutionTree& tree, ts::Node node);
void rulesHandler(Game& active, ExecutionTree& tree, ts::Node node);
void recursiveIdent(Game &active,std::vector<std::string>& identifiers, ts::Node node);



ExpressionNode parseExpression(Game &active, ExecutionTree& tree,ts::Node node);
ExpressionNode parseBuiltIn(Game &active, ExecutionTree& tree,ts::Node node);
void recursiveIdent(Game &active,std::vector<std::string>& identifiers, ts::Node node);
ExpressionNode parseComparison(Game &active, ExecutionTree& tree,ts::Node node);