#pragma once
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class AssignmentNode : public StatementNode {
public:
    explicit AssignmentNode(std::vector<std::unique_ptr<ASTNode>> children)
        : StatementNode("", "assign", std::move(children)) {}

    void accept(AstVisitor* visitor) override;

    ExpressionNode* getExpr();
    std::string getVar();
};
