#pragma once
#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/FactorNode.h"
class ConstNode : public FactorNode {
public:
    explicit ConstNode(std::string value) : FactorNode(value, "const") {}
};
