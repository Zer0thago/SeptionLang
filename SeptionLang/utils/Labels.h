#ifndef LABELS_H
#define LABELS_H

#include <string>
#include <unordered_map>

class Labels {
public:
    static void add(const std::string& label, int address) {
        labels[label] = address;
    }

    static int get(const std::string& label) {
        auto it = labels.find(label);
        if (it != labels.end()) {
            return it->second;
        }
        return -1;
    }

private:
    static std::unordered_map<std::string, int> labels;
};

inline std::unordered_map<std::string, int> Labels::labels;

#endif