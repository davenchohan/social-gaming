#include <cpp-tree-sitter.h>

#include <cassert>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

struct Value; 

class ExpressionMap {
   public:
    ExpressionMap(std::string_view source);
    Value find(std::string key) const;
    void sourceToMap(const ts::Node &node);

   private:
    std::map<std::string, Value> expressionMap;
    std::string_view source;
};

struct Value {
    Value();
    Value(ts::Node node, std::string_view source);
    std::variant<int, bool, std::string, std::pair<int, int>, std::vector<Value>, std::map<std::string, Value>> value;
};

