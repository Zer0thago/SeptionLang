#include "Stack.h"
#include <iostream>

void Stack::push(int value) {
    buf.push_back(value);
}

void Stack::push(const std::string& value) {
    buf.push_back(value);
}

Stack::Value Stack::pop() {
    if (buf.empty()) {
        std::cout << std::endl;
        throw std::runtime_error("Stack underflow");
    }
    Value val = buf.back();
    buf.pop_back();
    return val;
}

const Stack::Value& Stack::top() const {
    if (buf.empty()) {
        std::cout << std::endl;
        throw std::runtime_error("Stack empty");
    }
    return buf.back();
}

void Stack::push(const Value& value) {
    buf.push_back(value);
}
