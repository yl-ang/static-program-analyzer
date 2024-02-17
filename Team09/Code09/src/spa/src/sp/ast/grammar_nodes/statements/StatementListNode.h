#include "AstNode.h"

class StatementListNode : public ASTNode {
public:
    StatementListNode() : ASTNode("", "stmtList") {}
};
