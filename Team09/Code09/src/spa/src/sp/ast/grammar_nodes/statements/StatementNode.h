#include <string>
#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"

class StatementNode : public ASTNode {
public:
    StatementNode(std::string value, std::string type, std::vector<std::unique_ptr<ASTNode>> children)
        : ASTNode(value, type, std::move(children)) {}
    StatementNode() : ASTNode("", "") {}
};
