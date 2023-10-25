#include <cpp-tree-sitter.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <cassert>
#include <cstdio>
#include <memory>


class SGParser {
   public:
    SGParser(const std::string filepath);
    json setupToJson();
    json configToJson();
    ts::Node getRoot(); 
    ts::Node getConfig();
    ts::Node getConstants();
    ts::Node getVar();
    ts::Node getPerPlayer();
    ts::Node getPerAudience();
    ts::Node getRules();

   private:
    static const std::string rootFields[6];
    static const std::string configFields[2];
    static const std::string setupFields[5];
    std::string source;

    ts::Tree *tree;

   

    std::string getChildStrByField(ts::Node parent, const std::string field);
};
