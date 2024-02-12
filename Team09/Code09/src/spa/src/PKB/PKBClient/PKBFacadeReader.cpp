#include "PKBFacadeReader.h"
#include "PKB/PKB.h"

PKBFacadeReader::PKBFacadeReader(PKB& pkbReference) {
    this->pkbReference = &pkbReference;
}

std::unordered_set<std::string> PKBFacadeReader::getVariables() const {
    return pkbReference->variableStore->getAllEntities();
}

std::unordered_set<Constant> PKBFacadeReader::getConstants() const {
    return pkbReference->constantStore->getAllEntities();
}

std::unordered_set<Procedure> PKBFacadeReader::getProcedures() const {
    return pkbReference->procedureStore->getAllEntities();
}

bool PKBFacadeReader::containsVariable(const Variable& variable) const {
    return pkbReference->variableStore->hasEntity(variable);
}

bool PKBFacadeReader::containsConstant(const Constant& constant) const {
    return pkbReference->constantStore->hasEntity(constant);
}

bool PKBFacadeReader::containsProcedure(const Procedure& procedure) const {
    return pkbReference->procedureStore->hasEntity(procedure);
}

std::unordered_set<Stmt> PKBFacadeReader::getStmts() const {
    return pkbReference->stmtStore->getStatements();
}

Stmt *PKBFacadeReader::getStatementByStmtNum(StmtNum stmtNum) const {
    return pkbReference->stmtStore->getStatementByStmtNum(stmtNum);
}

std::vector<Stmt *> PKBFacadeReader::getStatementsByType(StatementType type) const {
    return pkbReference->stmtStore->getStatementsByType(type);
}

std::optional<StmtNum> PKBFacadeReader::getFollower(StmtNum s1) const {
    return pkbReference->followsStore->getFollower(s1);
}

std::optional<StmtNum> PKBFacadeReader::getFollowee(StmtNum s2) const {
    return pkbReference->followsStore->getFollowee(s2);
}

std::unordered_set<StmtNum> PKBFacadeReader::getFolloweesStar(StmtNum s) const {
    return pkbReference->followsStore->getFolloweesStar(s);
}

std::unordered_set<StmtNum> PKBFacadeReader::getFollowersStar(StmtNum s) const {
    return pkbReference->followsStore->getFollowersStar(s);
}

bool PKBFacadeReader::containsFollowRelationship(StmtNum s1, StmtNum s2) const {
    return pkbReference->followsStore->containsFollowRelationship(s1, s2);
}

bool PKBFacadeReader::containsFollowStarRelationship(StmtNum s1, StmtNum s2) const {
    return pkbReference->followsStore->containsFollowStarRelationship(s1, s2);
}
