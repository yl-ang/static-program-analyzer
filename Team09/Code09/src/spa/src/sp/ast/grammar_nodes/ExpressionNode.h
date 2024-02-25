#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/Utils.h"
#include "sp/ast/AstNode.h"
/*
Covers addition and subtraction

*/
class ExpressionNode : public ASTNode {
public:
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type, std::vector<std::unique_ptr<ASTNode>> children, int stmtNumber)
        : ASTNode("", getLexicalEnumString(type), std::move(children), stmtNumber) {}
    ExpressionNode(std::string value, std::string type, int stmtNumber) : ASTNode(value, type, {}, stmtNumber) {}
};
