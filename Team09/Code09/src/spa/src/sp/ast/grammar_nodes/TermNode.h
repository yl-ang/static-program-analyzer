#pragma once

#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
/*
Covers multiplication, modulus and div
*/
class TermNode : public ExpressionNode {
public:
    explicit TermNode(LEXICAL_TOKEN_TYPE type) : ExpressionNode(type) {}
    TermNode(std::string value, std::string type)
        : ExpressionNode(value, type) {}
};
