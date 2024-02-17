#include <string>

#include "AstNode.h"
class ConstantNode : public ASTNode {
    explicit ConstantNode(std::string value) : ASTNode(value, "const") {}
};
