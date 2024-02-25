#include "DesignExtractor.h"

#include <vector>

void DesignExtractor::writePKB(PKBFacadeWriter* writer) {
    writer->setVariables(getVariables());
    writer->setConstants(getConstants());
    writer->setProcedures(getProcedures());
    writer->setStmts(getStatements());
    writer->setFollowsStore(getFollows());
    writer->setParentStore(getParent());
    writer->setUsesStore(getUses());
    writer->setModifiesStore(getModifies());
    writer->setPatternStore(getPattern());
}

void DesignExtractor::extract(std::unique_ptr<ASTNode> root) {
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

void DesignExtractor::dfsVisit(std::unique_ptr<ASTNode> node, AstVisitor* visitor) {
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisit(child, visitor);
    }
    return;
}

std::unordered_set<std::string> DesignExtractor::getVariables() {
    return entityExtractor->getVariables();
}

std::unordered_set<std::string> DesignExtractor::getConstants() {
    return entityExtractor->getConstants();
}

std::unordered_set<std::string> DesignExtractor::getProcedures() {
    return entityExtractor->getProcedures();
}

std::unordered_set<Stmt> DesignExtractor::getStatements() {
    return entityExtractor->getStatements();
}
std::unordered_set<std::pair<StmtNum, StmtNum>> DesignExtractor::getFollows() {
    return followsExtractor->getFollows();
}

std::unordered_set<std::pair<StmtNum, StmtNum>> DesignExtractor::getParent() {
    return parentExtractor->getParent();
}

std::unordered_set<std::pair<StmtNum, Variable>> DesignExtractor::getUses() {
    return usesExtractor->getUses();
}

std::unordered_set<std::pair<StmtNum, Variable>> DesignExtractor::getModifies() {
    return modifiesExtractor->getModifies();
}

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> DesignExtractor::getPattern() {
    return patternExtractor->getPattern();
}
