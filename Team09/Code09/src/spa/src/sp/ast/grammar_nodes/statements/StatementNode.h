#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"

class StatementNode : public ASTNode {
public:
    StatementNode(std::string value, std::string type, int statementNumber,
                  std::vector<std::shared_ptr<ASTNode>> children)
        : ASTNode(value, type, (children), statementNumber) {
    }
    StatementNode() : ASTNode("", "") {}
};
