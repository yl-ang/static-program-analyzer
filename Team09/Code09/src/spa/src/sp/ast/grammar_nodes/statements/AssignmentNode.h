#pragma once
#include <string>
#include <typeinfo>
#include <vector>

#include "sp/ast/grammar_nodes/statements/StatementNode.h"
class AssignmentNode : public StatementNode {
public:
    AssignmentNode() : StatementNode("", "assign") {}

    void accept(AstVisitor* visitor) override;

    ExpressionNode* getExpr();
    std::string getVar();
};
