#pragma once

#include <memory>
#include <string>
#include <vector>

#include "sp/Utils.h"
#include "sp/ast/AstNode.h"

/*
Covers every expression including conditionals. Since NOT is a single conditional with 1 child, the right child becomes
a nullptr when called with the appropriate constructor. In otherwords, expression node will always have a left child.
*/
class ExpressionNode : public ASTNode {
private:
    std::vector<std::string> variables;
    std::vector<std::string> constants;
    void findVariables(const std::vector<std::shared_ptr<ASTNode>>& children);
    void findConstants(const std::vector<std::shared_ptr<ASTNode>>& children);

public:
    // generic case
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type, std::shared_ptr<ExpressionNode> left,
                            std::shared_ptr<ExpressionNode> right, int stmtNumber)
        : ASTNode("", getLexicalEnumString(type), {left, right}, stmtNumber),
          left(left),
          right(right),
          type(getLexicalEnumString(type)),
          value(getLexicalEnumString(type)) {}

    // handle the case for NOT
    ExpressionNode(LEXICAL_TOKEN_TYPE type, std::shared_ptr<ExpressionNode> singleExpression, int stmtNumber)
        : ASTNode("", getLexicalEnumString(type), {singleExpression}, stmtNumber),
          type(getLexicalEnumString(type)),
          value(getLexicalEnumString(type)),
          left(singleExpression),
          right(nullptr) {}

    ExpressionNode(std::string value, std::string type, int stmtNumber)
        : ASTNode(value, type, {}, stmtNumber), value(value), type(type) {}

    void accept(AstVisitor* visitor) override;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
    std::string value;
    std::string type;

    std::vector<std::string> getVars();
    std::vector<std::string> getConsts();
};
