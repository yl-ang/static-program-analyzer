#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class PrintNode : public StatementNode {
public:
    explicit PrintNode(int statementNumber, std::vector<std::unique_ptr<ASTNode>> children)
        : StatementNode("", "print", statementNumber, std::move(children)) {}

    void accept(AstVisitor* visitor) override;

    std::string getVar();
};
