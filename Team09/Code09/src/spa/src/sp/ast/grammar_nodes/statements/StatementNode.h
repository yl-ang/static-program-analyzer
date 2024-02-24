#pragma once
#include <string>

#include "sp/ast/AstNode.h"

class StatementNode : public ASTNode {
public:
    StatementNode(std::string value, std::string type) : ASTNode(value, type) {}
    StatementNode() : ASTNode("", "") {}
};
