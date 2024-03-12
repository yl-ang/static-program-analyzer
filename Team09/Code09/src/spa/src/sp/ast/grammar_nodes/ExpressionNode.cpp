#include "ExpressionNode.h"

#include "ConstantNode.h"
#include "VariableNode.h"

void ExpressionNode::accept(AstVisitor* visitor) {
    visitor->visitExpression(this);
}

std::vector<std::string> ExpressionNode::getVars() {
    findVariables(this->getChildren());
    return variables;
}

std::vector<std::string> ExpressionNode::getConsts() {
    findConstants(this->getChildren());
    return constants;
}

// Function to traverse through all children nodes and retrieve variable nodes
void ExpressionNode::findVariables(const std::vector<std::shared_ptr<ASTNode>>& children) {
    if (children.size() == 0) {
        if (auto variableNodePtr = dynamic_cast<VariableNode*>(this)) {
            variables.push_back(variableNodePtr->getValue());
        }
    }
    for (auto& child : children) {
        // Use dynamic_cast to check if the child node is of type VariableNode
        if (auto variableNodePtr = dynamic_cast<VariableNode*>(child.get())) {
            variables.push_back(variableNodePtr->getValue());
        }
        // Recursively traverse child nodes
        findVariables(child->getChildren());
    }
}

// Function to traverse through all children nodes and retrieve constant nodes
void ExpressionNode::findConstants(const std::vector<std::shared_ptr<ASTNode>>& children) {
    if (children.size() == 0) {
        if (auto constantNodePtr = dynamic_cast<ConstantNode*>(this)) {
            constants.push_back(constantNodePtr->getValue());
        }
    }
    for (auto& child : children) {
        // Use dynamic_cast to check if the child node is of type ConstantNode
        if (auto constantNodePtr = dynamic_cast<ConstantNode*>(child.get())) {
            constants.push_back(constantNodePtr->getValue());
        }
        // Recursively traverse child nodes
        findConstants(child->getChildren());
    }
}

bool ExpressionNode::match(std::shared_ptr<Matchable> input) {
    // Check if the input is of type ExpressionNode
    if (auto expressionNodePtr = std::dynamic_pointer_cast<ExpressionNode>(input)) {
        if (this->getType() != expressionNodePtr->getType() || this->getValue() != expressionNodePtr->getValue() ||
            this->getChildren().size() != expressionNodePtr->getChildren().size()) {
            return false;
        }

        // Match all its children.
        for (size_t i = 0; i < this->getChildren().size(); ++i) {
            const std::shared_ptr<ASTNode>& thisChild = this->getChildren()[i];
            const std::shared_ptr<ASTNode>& otherChild = expressionNodePtr->getChildren()[i];

            std::shared_ptr<ExpressionNode> thisChildPtr = std::static_pointer_cast<ExpressionNode>(thisChild);
            std::shared_ptr<ExpressionNode> otherChildPtr = std::static_pointer_cast<ExpressionNode>(otherChild);

            if (!thisChildPtr || !otherChildPtr) {
                // For some strange reason, one of the children is not an expression node. This should not happen.
                throw std::exception(
                    "ExpressionNode has a child that is not an ExpressionNode or some other derived class of "
                    "itself.");
            }

            if (!(thisChildPtr->match(otherChildPtr))) {
                return false;
            }
        }

        return true;
    }

    return false;
}
