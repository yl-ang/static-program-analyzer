#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "sp/tokenizer/Token.h"

class ASTNode {
    std::vector<std::unique_ptr<ASTNode>> children;
    std::string value;
    std::string type;
    int stmtNumber;

public:
    ASTNode(std::string value, std::string type, std::vector<std::unique_ptr<ASTNode>> children = {},
            int stmtNumber = -1)
        : value(value), type(type), children(std::move(children)), stmtNumber(stmtNumber) {}

    std::string getType() const {
        return type;
    }

    std::string getValue() const {
        return value;
    }
    friend bool operator==(const ASTNode& lhs, const ASTNode& rhs) {
        // Compare the easy to compare members
        if (lhs.getValue() != rhs.getValue() || lhs.getType() != rhs.getType() ||
            lhs.children.size() != rhs.children.size()) {
            return false;
        }

        // Now we know that lhs and rhs have the same number of children
        // Compare each child of lhs with each child of rhs
        for (size_t i = 0; i < lhs.children.size(); ++i) {
            if (!(*(lhs.children[i]) == *(rhs.children[i]))) {
                return false;
            }
        }

        // If we've gotten this far, everything is equal
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const ASTNode& obj) {
        os << "{ \"value\": \"" << obj.value << "\", "
           << "\"type\": \"" << obj.type << "\", "
           << "\"children\": [";

        for (size_t i = 0; i < obj.children.size(); ++i) {
            os << "[" << std::move(obj.children[i]) << "]";
            if (i != obj.children.size() - 1)
                os << ", ";
        }
        os << "] }";
        return os;
    }
};
