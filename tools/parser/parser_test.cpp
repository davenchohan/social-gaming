#include "parser_test.h"

#include <cpp-tree-sitter.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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
ts::Node SGParser::getVar() {
    return tree->getRootNode().getNamedChild(1);
}
ts::Node SGParser::getPerPlayer() {
    return tree->getRootNode().getNamedChild(2);
}
ts::Node SGParser::getPerAudience() {
    return tree->getRootNode().getNamedChild(3);
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
/**
int main() {
    SGParser p("./lib/gameSpecs/empty_spec.txt");
    std::cout << p.configToJson().dump();
}
*/