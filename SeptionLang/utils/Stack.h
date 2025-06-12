#pragma once
#include <stdexcept>
#include <vector>
#include <variant>
#include <string>

class Stack {
    using Value = std::variant<int, std::string>;
    std::vector<Value> buf;

public:
    void push(int value);
    void push(const std::string& value);
    void push(const Value& value);
    Value pop();
    const Value& top() const;
};
