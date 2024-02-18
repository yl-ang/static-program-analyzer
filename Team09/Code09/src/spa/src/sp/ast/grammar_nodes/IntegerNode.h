#pragma once
#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"
class IntegerNode : public FactorNode {
public:
    explicit IntegerNode(std::string value) : FactorNode(value, "const") {}
};
