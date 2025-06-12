#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class ProgramData {
public:
    static std::vector<std::string> Program;
    static std::vector<int> ReturnStack;
private:
    static std::unordered_map<std::string, int> labels;
};
