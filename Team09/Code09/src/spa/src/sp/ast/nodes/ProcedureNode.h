#include <string>

#include "AstNode.h"

class ProcedureNode : public ASTNode {
    explicit ProcedureNode(std::string name) : ASTNode(name, "proc") {}
};
