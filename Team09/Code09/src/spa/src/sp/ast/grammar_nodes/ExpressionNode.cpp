#include "ExpressionNode.h"

void ExpressionNode::accept(AstVisitor* visitor) {
    visitor->visitExpression(this);
}

std::vector<std::string> ExpressionNode::getVars() {
    findVariables(getChildren());
    return variables;
}

std::vector<std::string> ExpressionNode::getConsts() {
    findConstants(getChildren());
    return constants;
}

// Function to traverse through all children nodes and retrieve variable nodes
void ExpressionNode::findVariables(std::vector<std::unique_ptr<ASTNode>> children) {
    for (std::unique_ptr<ASTNode> child : children) {
        if (typeid(child) == typeid(VariableNode)) {
            variables.push_back(child->getValue());
        }
        // Recursively traverse child nodes
        findVariables(child->getChildren());
    }
    return;
}

// Function to traverse through all children nodes and retrieve constant nodes
void ExpressionNode::findConstants(std::vector<std::unique_ptr<ASTNode>> children) {
    for (std::unique_ptr<ASTNode> child : children) {
        if (typeid(child) == typeid(ConstantNode)) {
            constants.push_back(child->getValue());
        }
        // Recursively traverse child nodes
        findConstants(child->getChildren());
    }
    return;
}
