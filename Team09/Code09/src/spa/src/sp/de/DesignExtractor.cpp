#include "DesignExtractor.h"

#include <vector>

void DesignExtractor::writePKB(PKBFacadeWriter* writer) {
    writer->setVariables(entityExtractor->getVariables());
    writer->setConstants(entityExtractor->getConstants());
    writer->setProcedures(entityExtractor->getProcedures());
    writer->setStmts(entityExtractor->getStatements());
    writer->setFollowsStore(followsExtractor->getFollows());
    writer->setParentStore(parentExtractor->getParent());
    writer->setUsesStore(usesExtractor->getUses());
    writer->setModifiesStore(modifiesExtractor->getModifies());
    writer->setPatternStore(patternExtractor->getPattern());
}

void DesignExtractor::extract(ASTNode* root) {
    this->entityExtractor = new EntityExtractor();
    this->followsExtractor = new FollowsExtractor();
    this->parentExtractor = new ParentExtractor();
    this->usesExtractor = new UsesExtractor();
    this->modifiesExtractor = new ModifiesExtractor();
    this->patternExtractor = new PatternExtractor();

    std::vector<AstVisitor*> visitors{entityExtractor, followsExtractor, parentExtractor, usesExtractor,
                                      modifiesExtractor};

    for (auto& visitor : visitors) {
        dfsVisit(root, visitor);
    }
}

void DesignExtractor::dfsVisit(ASTNode* node, AstVisitor* visitor) {
    if (!node)
        return;
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisit(child, visitor);
    }
}
