#pragma once

#include <string>

#include "sp/Utils.h"
#include "sp/ast/AstNode.h"

/*
Covers addition and subtraction
*/
class ExpressionNode : public ASTNode {
public:
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type) : ASTNode("", getLexicalEnumString(type)) {}

    ExpressionNode(std::string value, std::string type) : ASTNode(value, type) {}
    void accept(AstVisitor* visitor) override {
        visitor->visitExpression(this);
    }
};
