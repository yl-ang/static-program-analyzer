#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class ReadNode : public StatementNode {
public:
    explicit ReadNode(std::vector<std::shared_ptr<ASTNode>> children, int statementNumber)
        : StatementNode("", "read", statementNumber, (children)) {
    }

    void accept(AstVisitor* visitor) override;

    std::string getVar();
};
