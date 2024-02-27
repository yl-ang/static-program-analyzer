#pragma once
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../ExpressionNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class AssignmentNode : public StatementNode {
public:
    explicit AssignmentNode(std::vector<std::shared_ptr<ASTNode>> children, int statementNumber)
        : StatementNode("", "assign", statementNumber, (children)) {
    }

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<ExpressionNode> getExpr();
    std::string getVar();
};
