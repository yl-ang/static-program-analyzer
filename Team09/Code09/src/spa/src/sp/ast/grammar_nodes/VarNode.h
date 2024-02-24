#pragma once

#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"

class VarNode : public FactorNode {
public:
    explicit VarNode(std::string value) : FactorNode(value, "var") {}
};
