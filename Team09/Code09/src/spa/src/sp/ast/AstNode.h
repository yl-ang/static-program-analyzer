#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "AstNodeInterface.h"
#include "sp/tokenizer/Token.h"

class ASTNode : public AstNodeInterface {
    std::vector<ASTNode> children;
    std::string value;
    std::string type;
    int stmtNum;

public:
    ASTNode(int stmtNum, std::string value, std::string type, std::vector<ASTNode> children = {})
        : stmtNum(stmtNum), value(value), type(type), children(children) {}

    void add_child(ASTNode child) {
        children.push_back(child);
    }

    // equality function override
    friend bool operator==(const ASTNode& lhs, const ASTNode& rhs) {
        return lhs.value == rhs.value && lhs.type == rhs.type && lhs.children == rhs.children;
    }

    int getStmtNum() const {
        return stmtNum;
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
