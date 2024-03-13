#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AstNodeInterface.h"

class ASTNode : public AstNodeInterface {
    std::vector<std::shared_ptr<ASTNode>> children;
    std::string value;
    std::string type;
    int stmtNumber;

public:
    ASTNode(std::string value, std::string type, std::vector<std::shared_ptr<ASTNode>> children = {},
            int stmtNumber = -1)
        : value(value), type(type), children(children), stmtNumber(stmtNumber) {}

    std::string getType() const {
        return type;
    }

    std::string getValue() const {
        return value;
    }

    int getStmtNumber() const {
        return stmtNumber;
    }

    const std::vector<std::shared_ptr<ASTNode>>& getChildren() const {
        return children;
    }

    void accept(AstVisitor* visitor) override {}

    friend bool operator==(const ASTNode& lhs, const ASTNode& rhs) {
        // Compare the easy to compare members
        if (lhs.getValue() != rhs.getValue() || lhs.getType() != rhs.getType() ||
            lhs.children.size() != rhs.children.size() || lhs.getStmtNumber() != rhs.getStmtNumber()) {
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
           << "\"line number\": \"" << obj.stmtNumber << "\","
           << "\"children\": [";

        for (size_t i = 0; i < obj.children.size(); ++i) {
            os << "[" << *(obj.children[i]) << "]";
            if (i != obj.children.size() - 1)
                os << ", ";
        }
        os << "] }";
        return os;
    }
};
