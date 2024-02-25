#include "ExpressionNode.h"

void ExpressionNode::accept(AstVisitor* visitor) {
    visitor->visitExpression(this);
}

std::vector<std::string> ExpressionNode::getVars() {
    findVariables(getChildren());
    return variables;
}

// Function to traverse through all children nodes and retrieve variable nodes
void ExpressionNode::findVariables(std::vector<ASTNode*> children) {
    for (ASTNode* child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            variables.push_back(child->getValue());
        }
        // Recursively traverse child nodes
        findVariables(child->getChildren());
    }
}
