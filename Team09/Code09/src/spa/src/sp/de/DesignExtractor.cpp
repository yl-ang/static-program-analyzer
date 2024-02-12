#pragma once
#include "DesignExtractor.h"

std::unordered_set<ASTNode> DesignExtractor::extract(ASTNode* root) {
     if (!root) return;

    // DFS
    for (ASTNode* child : root->getChildren()) {
        extractVariables(child);
    }

    // Check if the current node is of type "var"
    if (root->getType() == "var") {
        variables.insert(root->getValue());
    }
}

void DesignExtractor::writePKB() {
    PKBwriter->setVariables(variables);
}
