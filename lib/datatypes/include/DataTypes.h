#pragma once
#include <string>
#include <vector>
#include <map>
/**This will all probably need to be changed I wrote this more as a template for what
 * I think this should generally look like so that I can instantiate basic versions of these while 
 * interpreting the trees
*/

enum class Types {
    BOOLEAN,
    INTEGER,
    STRING,
    ENUM,
    QUESTION_ANSWER,
    MULTIPLE_CHOICE,
    JSON,
    LIST,
};
class DataType {
    
public:
    DataType();

    std::string getIdentifier() const;
    bool isMutable() const;
    void setMutable(bool mutableType);
    Types getType();
    void setType(Types newType);
    virtual ~DataType() = default;

private:
    std::string identifier_;
    bool mutable_;
    Types type;
};

class IntegerType : public DataType {
public:
    IntegerType();
private: 
    int value;
};
class rangeType : public DataType {
public:
    rangeType();
private: 
    std::tuple<int,int> value;
};

class StringType : public DataType {
public:
    StringType();
};

class ListType : public DataType {
public:
    ListType();

    DataType* getElementType() const;
    void setElementType(DataType* elementType);

private:
    DataType* elementType;
};

class MapType : public DataType {
public:
    MapType();

    DataType* getKeyType() const;
    void setKeyType(DataType* keyType);
    DataType* getValueType() const;
    void setValueType(DataType* valueType);

private:
    DataType* keyType;
    DataType* valueType;
};