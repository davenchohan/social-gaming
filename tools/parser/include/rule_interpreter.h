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