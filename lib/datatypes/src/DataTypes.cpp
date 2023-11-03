#include <string>
#include <vector>
#include <map>
#include "DataTypes.h"


DataType::DataType() {

};

std::string DataType::getIdentifier() const {
return identifier_;
};

bool DataType::isMutable() const {
    return mutable_;
};
void DataType::setType(Types newType){
    type = newType;
};

void DataType::setMutable(bool mutableType) {
    mutable_ = mutableType;
};




IntegerType::IntegerType() {
    setType(Types::INTEGER);
};



StringType::StringType()  {
     setType(Types::STRING);
};


ListType::ListType() {
    setType(Types::LIST);
    elementType = elementType;
};

DataType* ListType::getElementType() const {
    return elementType;
};

void ListType::setElementType(DataType* elementType) {
    this->elementType = elementType;
}


MapType::MapType()  {}

DataType* MapType::getKeyType() const {
    return keyType;
}

void MapType::setKeyType(DataType* keyType) {
    this->keyType = keyType;
}

DataType* MapType::getValueType() const {
    return valueType;
}

void setValueType(DataType* valueType) {
}

