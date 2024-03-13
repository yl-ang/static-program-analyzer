#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
/*
Covers multiplication, modulus and div
*/
class TermNode : public ExpressionNode {
public:
    explicit TermNode(LEXICAL_TOKEN_TYPE type, std::vector<std::shared_ptr<ASTNode>> children, int stmtNumber)
        : ExpressionNode(type, (children), stmtNumber) {}
    TermNode(std::string value, std::string type, int stmtNumber) : ExpressionNode(value, type, stmtNumber) {}

    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;

    void accept(AstVisitor* visitor) override {
        visitor->visitTerm(this);
    }
};
