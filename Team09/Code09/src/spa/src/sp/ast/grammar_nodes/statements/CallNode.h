#pragma once
#include <string>
#include <vector>

#include "sp/ast/grammar_nodes/statements/StatementNode.h"

class CallNode : public StatementNode {
public:
    explicit CallNode(std::string procedureName, int statementNumber)
        : StatementNode(procedureName, "call", statementNumber, {}) {}

    void accept(AstVisitor* visitor) override;

    std::string getCalledProcedure();
};