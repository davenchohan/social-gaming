#ifndef GAMEVARIABLE_H
#define GAMEVARIABLE_H

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <stdexcept>

class GameVariable {
public:
    // Constructors
    GameVariable(const int& id);
    GameVariable(const std::string& name, const int id, const int& intValue);
    GameVariable(const std::string& name, const int id, const double& doubleValue);
    GameVariable(const std::string& name, const int id, const std::string& stringValue);

    // Type retrieval functions
    bool IsInt() const;
    bool IsDouble() const;
    bool IsString() const;

    // Templated getter for data
    // example use: variable.GetData<int>();
    template <typename T>
    T GetData() const {
        try {
            return std::get<T>(data);
        } catch (const std::bad_variant_access& e) {
            throw std::runtime_error("Requested type does not match the variant type.");
        }
    }

    std::string GetName() const;

    bool operator==(const GameVariable& other) const;
    bool operator<(const GameVariable& other) const;

    int GetVariableId() const;

private:
    std::variant<int, double, std::string> data;
    int id;
    std::string variableName;
};

#endif
