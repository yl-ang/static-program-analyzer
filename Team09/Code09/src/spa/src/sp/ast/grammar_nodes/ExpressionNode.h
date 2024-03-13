#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/Utils.h"
#include "sp/ast/AstNode.h"
#include "sp/ast/Matchable.h"

/*
Covers addition and subtraction
*/
class ExpressionNode : public ASTNode, public Matchable {
private:
    std::vector<std::string> variables;
    std::vector<std::string> constants;

public:
    explicit ExpressionNode(LEXICAL_TOKEN_TYPE type, std::vector<std::shared_ptr<ASTNode>> children, int stmtNumber)
        : ASTNode("", getLexicalEnumString(type), (children), stmtNumber) {}
    ExpressionNode(std::string value, std::string type, int stmtNumber) : ASTNode(value, type, {}, stmtNumber) {}

    void accept(AstVisitor* visitor) override;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;

    std::vector<std::string> getVars();
    std::vector<std::string> getConsts();

    void findVariables(const std::vector<std::shared_ptr<ASTNode>>& children);
    void findConstants(const std::vector<std::shared_ptr<ASTNode>>& children);

    // Matchable interface
    bool match(std::shared_ptr<Matchable> input) override;
};
