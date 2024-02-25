#pragma once
#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"
class ConstantNode : public FactorNode {
public:
    explicit ConstantNode(std::string value, int stmtNumber) : FactorNode(value, "const", stmtNumber) {}
};
