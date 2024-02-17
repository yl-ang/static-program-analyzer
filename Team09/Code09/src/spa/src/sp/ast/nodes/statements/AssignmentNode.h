#include "AstNode.h"

class AssignmentNode : public ASTNode {
    explicit AssignmentNode() : ASTNode("", "assign") {}
};
