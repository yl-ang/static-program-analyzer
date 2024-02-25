#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "sp/tokenizer/Token.h"

class ASTNode {
    std::vector<ASTNode> children;
    std::string value;
    std::string type;

public:
    ASTNode(std::string value, std::string type, std::vector<ASTNode> children = {})
        : value(value), type(type), children(children) {}

    void add_child(ASTNode child) {
        children.push_back(child);
    }

    // equality function override
    friend bool operator==(const ASTNode& lhs, const ASTNode& rhs) {
        return lhs.value == rhs.value && lhs.type == rhs.type && lhs.children == rhs.children;
    }

    std::string getType() const {
        return type;
    }

    std::string getValue() const {
        return value;
    }

    std::vector<ASTNode> getChildren() const {
        return children;
    }

    friend std::ostream& operator<<(std::ostream& os, const ASTNode& obj) {
        os << "{ \"value\": \"" << obj.value << "\", "
           << "\"type\": \"" << obj.type << "\", "
           << "\"children\": [";

        for (size_t i = 0; i < obj.children.size(); ++i) {
            os << obj.children[i];
            if (i != obj.children.size() - 1)
                os << ", ";
        }
        os << "] }";
        return os;
    }
};
