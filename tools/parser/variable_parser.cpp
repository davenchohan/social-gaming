#include "variable_parser.h"

#include <cpp-tree-sitter.h>

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

static void printChildren(const ts::Node& node) {
    ts::Cursor cursor = node.getCursor();
    if (cursor.gotoFirstChild()) {
        do {
            ts::Node curr = cursor.getCurrentNode();
            std::cout << "  " << curr.getType() << std::endl;
        } while (cursor.gotoNextSibling());
    }
}

struct printValue {
    int depth = 0;

    printValue() : depth{0} {}

    void printDepth(){
        std::cout << std::string(depth*2, ' ');
    }

    void operator()(const int& val) {
        printDepth();
        std::cout << "Integer " << val << std::endl;
    }
    void operator()(const bool& val) {
        printDepth();
        std::cout << "Boolean " << val << std::endl;
    }
    void operator()(const std::string& val) {
        printDepth();
        std::cout << "String " << val << std::endl;
    }
    void operator()(const std::pair<int, int>& val) {
        printDepth();
        std::cout << "Range " << val.first << ", " << val.second << std::endl;
    }
    void operator()(const std::vector<Value>& val) {
        printDepth();
        depth++;
        std::cout << "Vector " << std::endl;
        for (auto const& child : val) {
            std::visit(*this, child.value);
        }
        depth--;
    }
    void operator()(const std::map<std::string, Value>& val) {
        printDepth();
        depth++;
        std::cout << "Map " << std::endl;
        for (auto const& child : val) {
            printDepth();
            std::cout << "Key " << child.first << std::endl;
            std::visit(*this, child.second.value);
        }
        depth--;
    }
};

ExpressionMap::ExpressionMap(std::string_view source) : source{source} {}

Value ExpressionMap::find(std::string key) const {
    auto elem = expressionMap.find(key);
    if (elem == expressionMap.end()) {
        throw std::invalid_argument(key + " not supported");
    }
    return (elem->second);
}

void ExpressionMap::sourceToMap(const ts::Node& node) {
    ts::Node constantsNode = node.getChildByFieldName("constants").getChildByFieldName("map");
    ts::Node variablesNode = node.getChildByFieldName("variables").getChildByFieldName("map");
    ts::Node perPlayerNode = node.getChildByFieldName("per_player").getChildByFieldName("map");
    ts::Node perAudienceNode = node.getChildByFieldName("per_audience").getChildByFieldName("map");

    std::vector<ts::Node> nodes = {constantsNode, variablesNode, perPlayerNode, perAudienceNode};
    for(auto node : nodes){
        ts::Cursor cursor = node.getCursor();
        if (cursor.gotoFirstChild()) {
            do {
                ts::Node curr = cursor.getCurrentNode();
                if (curr.getType() == "map_entry") {
                    std::string key = std::string{curr.getChildByFieldName("key").getSourceRange(source)};
                    Value value = Value{curr.getChildByFieldName("value").getChild(0), source};
                    expressionMap[key] = value;
                }
            } while (cursor.gotoNextSibling());
        }
    }

}

Value::Value() {
    value = "ABSURD_VALUE";
}

Value::Value(ts::Node node, std::string_view source) {
    std::string_view type = node.getType();
    std::string_view content = node.getSourceRange(source);

    if (type == "boolean") {
        value = content == "true" ? true : false;
    } else if (type == "number") {
        value = std::stoi(std::string{content});
    } else if (type == "quoted_string") {
        value = std::string{content.substr(1, content.size() - 2)};
    } else if (type == "list_literal") {
        std::vector<Value> values;
        if(!node.getChildByFieldName("elements").isNull()){
            ts::Cursor cursor = node.getChildByFieldName("elements").getCursor();
            if (cursor.gotoFirstChild()) {
                do {
                    ts::Node curr = cursor.getCurrentNode();
                    if (curr.getType() == "expression") {
                        Value value = Value{curr.getChild(0), source};
                        values.push_back(value);
                    }
                } while (cursor.gotoNextSibling());
            }
        }
        value = values;
    } else if (type == "identifier") {
        value = std::string{content};
    } else if (type == "value_map") {
        std::map<std::string, Value> map; 
        ts::Cursor cursor = node.getCursor();
        if (cursor.gotoFirstChild()) {
            do {
                ts::Node curr = cursor.getCurrentNode();
                if (curr.getType() == "map_entry") {
                    std::string key = std::string{curr.getChildByFieldName("key").getSourceRange(source)};
                    Value value = Value{curr.getChildByFieldName("value").getChild(0), source};
                    map[key] = value;
                }
            } while (cursor.gotoNextSibling());
        }
        value = map;
    }
    // std::visit(printValue(), value);
}

int main() {
    ts::Language language = tree_sitter_socialgaming();
    ts::Parser parser{language};

    std::ifstream t("./lib/gameSpecs/rock_paper_scissors.txt");
    if (!t.is_open()) {
        throw("[ERROR] incorrect filepath");
    }
    std::string source((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());

    t.close();
    ts::Tree tree = parser.parseString(source);
    ts::Node root = tree.getRootNode();

    ExpressionMap map{source};
    map.sourceToMap(root);
    std::visit(printValue(), map.find("weapons2").value);
    std::visit(printValue(), map.find("something4").value);
    std::visit(printValue(), map.find("wins").value);
    std::visit(printValue(), map.find("winners").value);
    // Value value{tree.getRootNode().getChildByFieldName("configuration").getChildByFieldName("name"), source, map};
}
