#pragma once
#include <memory>
#include <string>
#include <vector>

#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class AssignmentNode : public StatementNode {
public:
    explicit AssignmentNode(std::shared_ptr<VariableNode> left, std::shared_ptr<ExpressionNode> right,
                            int statementNumber)
        : StatementNode("", "assign", statementNumber, {left, right}),
          variable(left),
          expression(right),
          statementNumber(statementNumber) {}

    std::shared_ptr<VariableNode> variable;
    std::shared_ptr<ExpressionNode> expression;
    int statementNumber;

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<ExpressionNode> getExpr();
    std::string getVar();
};
