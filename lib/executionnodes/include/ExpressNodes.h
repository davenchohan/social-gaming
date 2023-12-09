
#pragma once
#include "Nodes.h"
#include "Tree.h"
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <stdexcept>

enum class Builtins {
    SIZE,
    UPFROM,
    CONTAINS,
    COLLECT
};
enum class SimpleType {
    BOOLEAN,
    NUMBER,
    STRING,
    IDENTIFIER
};
enum class OpType {
    ADD,
    SUB,
    MULT,
    DIV
};
enum class CompType {
    EQ,
    GREATER,
    LESS,
    NOTEQ,
    GREATEREQ,
    LESSEQ,
    AND,
    OR
};

//general expression node type generic expression nodes should not be created. Expression node will implement a evaluate function to allow for expression evaluation
//_________________________________________________________________________________________________________________________________________________________________
class ExpressionNode {
public: 
    ExpressionNode();
    bool evaluate();
    std::vector<std::string> identifiers;
    void printIdents();
    virtual void print() =0; 
private:
    bool isFor;
    bool isSimple = false;
    //virtual std::variant<> getValue(); // this will be implemented for all expression node types to allow for node evaluation
};

class IdentNode : public ExpressionNode {
    public:
        IdentNode(std::vector<std::string> identifiers);
        void print();
    private:
        std::vector<std::string> identifiers;
};



class BuiltInNode : public ExpressionNode {
public:
    BuiltInNode(Builtins type,std::vector<std::string> identifiers, const std::vector<ExpressionNode*>& args);
    void print();
private:
    std::vector<std::string> identifiers;
    Builtins builtinType;
    std::vector<ExpressionNode*> args;
};

class OpExpressionNode : public ExpressionNode{
    public: 
    OpExpressionNode();
    OpExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , OpType operation);
    template <SimpleType T>
    auto evaluate() const;
    void print();
    private:
    ExpressionNode* lhs;
    ExpressionNode* rhs;
    OpType operation;
    
};
class NotNode : public ExpressionNode{
    public: 
    NotNode(ExpressionNode* express);
    template <SimpleType T>
    auto evaluate() const;
    void print();
    private:
    ExpressionNode* express;

};

class CompExpressionNode : public ExpressionNode{
    public: 
    CompExpressionNode();
    CompExpressionNode(ExpressionNode* lhs, ExpressionNode* rhs , CompType comparison);
    ExpressionNode* getLeft() {
        return lhs;
    }
    ExpressionNode* getRight() {
        return rhs;
    }
    template <SimpleType T>
    bool evaluate() const;
    void print();
    private:
    ExpressionNode* lhs;
    ExpressionNode* rhs;
    CompType comparison;

};




class SimpleExpression : public ExpressionNode{
    public:
    SimpleExpression(SimpleType type, std::string value);
    void print();
    std::string getValue() {
        return value;
    }
    template <SimpleType T>
    auto evaluate() const;
    private:
    SimpleType type;
    std::string value;

};

template<> inline auto SimpleExpression::evaluate<SimpleType::NUMBER>() const {
    return std::stoi(value);
}

template<> inline auto SimpleExpression::evaluate<SimpleType::STRING>() const {
    return value;
}

template<> inline auto SimpleExpression::evaluate<SimpleType::BOOLEAN>() const {
    return (value == "true");
}

template<>
inline bool CompExpressionNode::evaluate<SimpleType::NUMBER>() const {
    switch (comparison) {
        case CompType::EQ:
            return lhs->evaluate() == rhs->evaluate();
        case CompType::GREATER:
            return lhs->evaluate() > rhs->evaluate();
        case CompType::LESS:
            return lhs->evaluate() < rhs->evaluate();
        case CompType::NOTEQ:
            return lhs->evaluate() != rhs->evaluate();
        case CompType::GREATEREQ:
            return lhs->evaluate() >= rhs->evaluate();
        case CompType::LESSEQ:
            return lhs->evaluate() <= rhs->evaluate();
        // Handle logical AND and OR if needed
        default:
            // Handle other comparison types or throw an exception for unsupported types
            throw std::runtime_error("Unsupported comparison type for SimpleType::NUMBER");
    }
}

template<>
inline bool CompExpressionNode::evaluate<SimpleType::STRING>() const {
    switch (comparison) {
        case CompType::EQ:
            return lhs->evaluate() == rhs->evaluate();
        // Handle other string comparison types if needed
        default:
            // Handle other comparison types or throw an exception for unsupported types
            throw std::runtime_error("Unsupported comparison type for SimpleType::STRING");
    }
}

template<>
inline bool CompExpressionNode::evaluate<SimpleType::BOOLEAN>() const {
    switch (comparison) {
        case CompType::EQ:
            return lhs->evaluate() == rhs->evaluate();
        // Handle other boolean comparison types if needed
        default:
            // Handle other comparison types or throw an exception for unsupported types
            throw std::runtime_error("Unsupported comparison type for SimpleType::BOOLEAN");
    }
}

template <>
inline auto NotNode::evaluate<SimpleType::BOOLEAN>() const {
    return  (!express->evaluate());
}

template <>
inline auto NotNode::evaluate<SimpleType::NUMBER>() const {
    return - (express->evaluate());
}

template <>
inline auto OpExpressionNode::evaluate<SimpleType::NUMBER>() const {
    switch (operation) {
        case OpType::ADD:
            return lhs->evaluate() + rhs->evaluate();
        case OpType::SUB:
            return lhs->evaluate() - rhs->evaluate();
        case OpType::MULT:
            return lhs->evaluate() * rhs->evaluate();
        case OpType::DIV:
            return lhs->evaluate() / rhs->evaluate();
        // Handle other operation types if needed
        default:
            // Handle other operation types or throw an exception for unsupported types
            throw std::runtime_error("Unsupported operation type for SimpleType::NUMBER");
    }
}

// Inline definition for SimpleType::STRING
template <>
inline auto OpExpressionNode::evaluate<SimpleType::STRING>() const {
    // Handle string operations if needed
    switch (operation) {
        case OpType::ADD:
            return lhs->evaluate() + rhs->evaluate();

        default:
            throw std::runtime_error("Unsupported operation type for SimpleType::STRING");
    }
}

// Inline definition for SimpleType::BOOLEAN
template <>
inline auto OpExpressionNode::evaluate<SimpleType::BOOLEAN>() const {
    // Handle boolean operations if needed
    throw std::runtime_error("Unsupported operation type for SimpleType::BOOLEAN");
}