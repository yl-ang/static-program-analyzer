#include "DesignExtractor.h"

#include <sp/ast/AstNode.h>

#include <unordered_map>
#include <vector>

void DesignExtractor::writePKB(PKBFacadeWriter* writer) {
    writer->setVariables(getVariables());
    writer->setConstants(getConstants());
    writer->setProcedures(getProcedures());
    writer->setStmts(getStatements());
    writer->setFollowsStore(getFollows());
    writer->setParentStore(getParent());
    writer->setStatementModifiesStore(getModifies());
    writer->setProcedureModifiesStore(getProcedureModifies());
    writer->setStatementUsesStore(getUses());
    writer->setProcedureUsesStore(getProcedureUses());
    writer->setNextStore(getNext());
    writer->setCallStore(getCalls());
    writer->setCallStmtStore(getCallStmts());
    writer->setAssignPatternStore(isExactMatch, isPartialMatch, getAssignmentPattern());
    writer->setIfPatternStore(getIfPattern());
    writer->setWhilePatternStore(getWhilePattern());
    delete entityExtractor;
    delete procedureTracker;
    delete followsExtractor;
    delete parentExtractor;
    delete usesExtractor;
    delete modifiesExtractor;
    delete patternExtractor;
    delete callsExtractor;
    delete nextExtractor;
}

void DesignExtractor::extract(const std::shared_ptr<ProgramNode> root) {
    this->procedureTracker = new ProcedureTracker();
    dfsVisit(root, procedureTracker);
    this->entityExtractor = new EntityExtractor();
    this->followsExtractor = new FollowsExtractor();
    this->parentExtractor = new ParentExtractor();

    std::unordered_map<WhileNode*, std::unordered_set<std::pair<StmtNum, Variable>>> extractedUsesWhiles = {};
    std::unordered_map<IfNode*, std::unordered_set<std::pair<StmtNum, Variable>>> extractedUsesIfs = {};
    std::unordered_map<WhileNode*, std::unordered_set<std::pair<StmtNum, Variable>>> extractedModifiesWhiles = {};
    std::unordered_map<IfNode*, std::unordered_set<std::pair<StmtNum, Variable>>> extractedModifiesIfs = {};

    this->usesExtractor =
        new UsesExtractor(procedureTracker->getProcedures(), &this->procedureTracker->extractedUsesProcs,
                          &extractedUsesWhiles, &extractedUsesIfs);

    this->modifiesExtractor =
        new ModifiesExtractor(procedureTracker->getProcedures(), &this->procedureTracker->extractedModifiesProcs,
                              &extractedModifiesWhiles, &extractedModifiesIfs);

    this->patternExtractor = new PatternExtractor();
    this->nextExtractor = new NextExtractor();
    this->callsExtractor = new CallsExtractor();

    std::vector<AstVisitor*> visitors{entityExtractor,   followsExtractor, parentExtractor, usesExtractor,
                                      modifiesExtractor, patternExtractor, callsExtractor};

    for (auto& visitor : visitors) {
        dfsVisit(root, visitor);
#ifdef DEBUG_BUILD
        std::cout << "Visited" << std::endl;
#endif
    }

#ifdef DEBUG_BUILD
    std::cout << "Completed Visiting" << std::endl;
#endif

    for (auto procedure : root->children) {
        this->nextExtractor->buildCFG(procedure);
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

std::unordered_set<std::pair<Procedure, Variable>> DesignExtractor::getProcedureUses() {
    return usesExtractor->getProcedureUses();
}

std::unordered_set<std::pair<StmtNum, Variable>> DesignExtractor::getModifies() {
    return modifiesExtractor->getModifies();
}

std::unordered_set<std::pair<Procedure, Variable>> DesignExtractor::getProcedureModifies() {
    return modifiesExtractor->getProcedureModifies();
}

std::unordered_set<std::pair<StmtNum, StmtNum>> DesignExtractor::getNext() {
    return nextExtractor->getNextRelationships();
}

std::unordered_set<std::pair<Procedure, Procedure>> DesignExtractor::getCalls() {
    return callsExtractor->getCalls();
}

std::unordered_set<std::pair<Procedure, StmtNum>> DesignExtractor::getCallStmts() {
    return callsExtractor->getCallStmts();
}

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> DesignExtractor::getAssignmentPattern() {
    return patternExtractor->getAssignmentPattern();
}

std::unordered_set<std::pair<StmtNum, std::string>> DesignExtractor::getIfPattern() {
    return patternExtractor->getIfPattern();
}

std::unordered_set<std::pair<StmtNum, std::string>> DesignExtractor::getWhilePattern() {
    return patternExtractor->getWhilePattern();
}
