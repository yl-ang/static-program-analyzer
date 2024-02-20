#pragma once

#include <string>

#include "sp/ast/AstNode.h"
class ConstantNode : public ASTNode {
    explicit ConstantNode(std::string value) : ASTNode(value, "const") {}
};
