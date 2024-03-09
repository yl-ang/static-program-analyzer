#include "DesignExtractor.h"

#include <sp/ast/AstNode.h>

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
    writer->setNextStore(getNext());
}

void DesignExtractor::extract(const std::shared_ptr<ProgramNode> root) {
    this->entityExtractor = new EntityExtractor();
    this->followsExtractor = new FollowsExtractor();
    this->parentExtractor = new ParentExtractor();
    this->usesExtractor = new UsesExtractor();
    this->modifiesExtractor = new ModifiesExtractor();
    this->patternExtractor = new PatternExtractor();

    std::vector<AstVisitor*> visitors{entityExtractor, followsExtractor,  parentExtractor,
                                      usesExtractor,   modifiesExtractor, patternExtractor};

    for (auto& visitor : visitors) {
        dfsVisit(root, visitor);
    }

    for (auto procedure : root->getChildren()) {
        cfg.buildCFG(procedure);
    }
}

void DesignExtractor::dfsVisit(std::shared_ptr<ASTNode>&& node, AstVisitor* visitor) {
    if (!node) {
        return;
    }

    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisit((std::shared_ptr<ASTNode>&&)child, visitor);
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

std::unordered_set<std::pair<StmtNum, StmtNum>> DesignExtractor::getNext() {
    return cfg.nextRelationships;
}
