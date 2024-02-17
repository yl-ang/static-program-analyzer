#include <string>

#include "AstNode.h"
class NameNode : public ASTNode {
    explicit NameNode(std::string name) : ASTNode(name, "var") {}
};
