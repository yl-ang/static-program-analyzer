#include "DesignExtractor.h"

void DesignExtractor::extract(ASTNode root) {
    // DFS
    for (ASTNode child : root.getChildren()) {
        extract(child);
    }

    // Check if the current node is of type "var"
    if (root.getType() == "var") {
        variables.insert(root.getValue());
    }
}

void DesignExtractor::writePKB(PKBFacadeWriter writer) {
    writer.setVariables(variables);
}

void DesignExtractor::extract(ASTNode* root) {
    this->entityExtractor = new EntityExtractor();
    this->followsExtractor = new FollowsExtractor();
    this->parentExtractor = new ParentExtractor();
    this->usesExtractor = new UsesExtractor();
    this->modifiesExtractor = new ModifiesExtractor();
    std::vector<AstVisitor*> visitors{entityExtractor, followsExtractor, parentExtractor, usesExtractor,
                                      modifiesExtractor};

    for (auto& visitor : visitors) {
        dfsTraversal(root, visitor);
    }
}

void dfsTraversal(ASTNode* node, AstVisitor* visitor) {
    if (!node)
        return;
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsTraversal(child, visitor);
    }
}
