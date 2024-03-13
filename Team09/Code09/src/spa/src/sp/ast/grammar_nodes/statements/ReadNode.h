#pragma once
#include <memory>
#include <string>
#include <vector>

#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class ReadNode : public StatementNode {
public:
    explicit ReadNode(std::shared_ptr<VariableNode> variable, int statementNumber)
        : StatementNode("", "read", statementNumber, {variable}),
          variable(variable),
          statementNumber(statementNumber) {}

    void accept(AstVisitor* visitor) override;
    std::shared_ptr<VariableNode> variable;
    int statementNumber;
    std::string getVar();
};
