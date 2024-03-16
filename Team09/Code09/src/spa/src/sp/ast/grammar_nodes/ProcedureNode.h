#pragma once
#include <memory>
#include <string>

#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"

class ProcedureNode : public ASTNode {
public:
    ProcedureNode(std::string name, std::shared_ptr<StatementListNode> statements)
        : ASTNode(name, "proc", std::vector<std::shared_ptr<ASTNode>>({statements}), -1),
          statements(statements),
          name(name) {}
    std::shared_ptr<StatementListNode> statements;
    std::string name;

    void accept(AstVisitor* visitor) override;

    std::shared_ptr<StatementListNode> getStmtLstNode();
};
