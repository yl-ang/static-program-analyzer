#pragma once

#include <string>
#include <vector>

#include "sp/Utils.h"
#include "sp/ast/AstNode.h"

/*
Covers addition and subtraction
*/
class ExpressionNode : public ASTNode {
private:
    std::vector<std::string> variables;

public:
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type) : ASTNode("", getLexicalEnumString(type)) {}

    ExpressionNode(std::string value, std::string type) : ASTNode(value, type) {}
    void accept(AstVisitor* visitor) override;

    std::vector<std::string> getVars();
    void findVariables(std::vector<ASTNode*> children);
};
