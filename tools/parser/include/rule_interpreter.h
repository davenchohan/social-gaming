#include <cpp-tree-sitter.h>
#include "Game.h"
#include "Player.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <cassert>
#include <cstdio>
#include <memory>


enum class DecisionTypes {
    FOR,
    WHILE,
    PARALLEL_FOR,
    MATCH,
    INPARALLEL,
    TEXT,
    CHOICE,
    RANGE,
    VOTE,
    EXTEND,
    REVERSE,
    SHUFFLE,
    SORT,
    DISCARD,
    TIME
};

DecisionTypes getCaseValue(std::string_view type);

bool isControlType(std::string type);
void forLoopHandler(Game& active, ts::Node node);
void whileLoopHanler(Game& active, ts::Node node);
void parallel_forHandler(Game& active, ts::Node node);
void matchHandler(Game& active, ts::Node node);
void inparallelHandler(Game& active, ts::Node node);
void handleControlType(Game& active, ts::Node node);

void handleText();
void handleChoice();
void handleRange();
void handleVote();
bool isHumanInput(std::string type);
void handleInput(Game& active, ts::Node node);

void handleExtend();
void handleReverse();
void handleShuffle();
void handleSort();
void handleDiscard();
bool isListType(std::string type);
void handleListOperation(Game& active, ts::Node node);

bool isTiming(std::string type);
void handleTiming(Game& active, ts::Node node);

void ruleVisitor(Game& active, ts::Node node);
void bodyHandler(Game& active, ts::Node node);
void rulesHandler(Game& active, ts::Node node);