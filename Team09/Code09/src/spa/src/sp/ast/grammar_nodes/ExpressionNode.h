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
private:
    std::vector<std::string> variables;
    std::vector<std::string> constants;

public:
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type, std::vector<std::unique_ptr<ASTNode>> children, int stmtNumber)
        : ASTNode("", getLexicalEnumString(type), std::move(children), stmtNumber) {}
    ExpressionNode(std::string value, std::string type, int stmtNumber) : ASTNode(value, type, {}, stmtNumber) {}

    void accept(AstVisitor* visitor) override;

    std::vector<std::string> getVars();
    std::vector<std::string> getConsts();

    void findVariables(const std::vector<std::unique_ptr<ASTNode>>& children);
    void findConstants(const std::vector<std::unique_ptr<ASTNode>>& children);
};
