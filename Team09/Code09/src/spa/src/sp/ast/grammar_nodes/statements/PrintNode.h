#pragma once
#include <memory>
#include <string>

#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class PrintNode : public StatementNode {
public:
    explicit PrintNode(std::shared_ptr<VariableNode> variable, int statementNumber)
        : StatementNode("", "print", statementNumber, {variable}),
          variable(variable),
          statementNumber(statementNumber) {}

    void accept(AstVisitor* visitor) override {
        visitor->visitPrint(this);
    }
    std::shared_ptr<VariableNode> variable;
    int statementNumber;
};
