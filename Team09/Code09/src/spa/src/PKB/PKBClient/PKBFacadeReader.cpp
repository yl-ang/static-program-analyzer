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

Stmt* PKBFacadeReader::getStatementByStmtNum(StmtNum stmtNum) const {
    return pkbReference->stmtStore->getStatementByStmtNum(stmtNum);
}

std::unordered_set<Stmt*> PKBFacadeReader::getStatementsByType(StatementType type) const {
    return pkbReference->stmtStore->getStatementsByType(type);
}

std::optional<StmtNum> PKBFacadeReader::getFollower(StmtNum s1) {
    return pkbReference->followsStore->getFollower(s1);
}

std::optional<StmtNum> PKBFacadeReader::getFollowee(StmtNum s2) {
    return pkbReference->followsStore->getFollowee(s2);
}

std::unordered_set<StmtNum> PKBFacadeReader::getFollowersStar(StmtNum s) {
    return pkbReference->followsStore->getFollowersStar(s);
}

std::unordered_set<StmtNum> PKBFacadeReader::getFolloweesStar(StmtNum s) {
    return pkbReference->followsStore->getFolloweesStar(s);
}

bool PKBFacadeReader::hasFollowRelationship(StmtNum s1, StmtNum s2) {
    return pkbReference->followsStore->hasFollowRelationship(s1, s2);
}

bool PKBFacadeReader::hasFollowStarRelationship(StmtNum s1, StmtNum s2) {
    return pkbReference->followsStore->hasFollowStarRelationship(s1, s2);
}

bool PKBFacadeReader::hasFollowRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    return pkbReference->followsStore->hasFollowRelationship(arg1, arg2);
}

bool PKBFacadeReader::hasFollowStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2) {
    return pkbReference->followsStore->hasFollowStarRelationship(arg1, arg2);
}
