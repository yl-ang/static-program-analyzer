#include "PKBFacadeReader.h"

#include "PKB/PKB.h"

PKBFacadeReader::PKBFacadeReader(PKB &pkbReference) {
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

std::unordered_set<Stmt> PKBFacadeReader::getStmts() const {
    return pkbReference->stmtStore->getStatements();
}

std::optional<Stmt> PKBFacadeReader::getStatementByStmtNum(StmtNum stmtNum) const {
    return pkbReference->stmtStore->getStatementByStmtNum(stmtNum);
}

std::unordered_set<Stmt> PKBFacadeReader::getStatementsByType(StatementType type) const {
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

bool PKBFacadeReader::hasFollowRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->followsStore->hasFollowRelationship(arg1, arg2);
}

bool PKBFacadeReader::hasFollowStarRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->followsStore->hasFollowStarRelationship(arg1, arg2);
}

std::optional<StmtNum> PKBFacadeReader::getParent(StmtNum child) {
    return pkbReference->parentStore->getParent(child);
}

std::unordered_set<StmtNum> PKBFacadeReader::getChildren(StmtNum parent) {
    return pkbReference->parentStore->getChildren(parent);
}

std::unordered_set<StmtNum> PKBFacadeReader::getChildrenStar(StmtNum parent) {
    return pkbReference->parentStore->getChildrenStar(parent);
}

std::unordered_set<StmtNum> PKBFacadeReader::getParentsStar(StmtNum child) {
    return pkbReference->parentStore->getParentsStar(child);
}

bool PKBFacadeReader::hasParentRelationship(StmtNum parent, StmtNum child) {
    return pkbReference->parentStore->hasParentRelationship(parent, child);
}

bool PKBFacadeReader::hasParentStarRelationship(StmtNum parent, StmtNum child) {
    return pkbReference->parentStore->hasParentStarRelationship(parent, child);
}

bool PKBFacadeReader::hasParentRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->parentStore->hasParentRelationship(arg1, arg2);
}

bool PKBFacadeReader::hasParentStarRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->parentStore->hasParentStarRelationship(arg1, arg2);
}

std::unordered_set<Variable> PKBFacadeReader::getModifiesVariablesByStatement(StmtNum stmt) const {
    return pkbReference->modifiesStore->getVariablesByStatement(stmt);
}

std::unordered_set<StmtNum> PKBFacadeReader::getModifiesStatementsByVariable(const Variable &variable) const {
    return pkbReference->modifiesStore->getStatementsByVariable(variable);
}

bool PKBFacadeReader::hasStatementVariableModifiesRelationship(ClauseArgument &arg1, ClauseArgument &arg2) const {
    return pkbReference->modifiesStore->hasStatementVariableModifiesRelationship(arg1, arg2);
}

std::unordered_set<Variable> PKBFacadeReader::getModifiesVariablesByProcedure(const Procedure &procedure) const {
    return pkbReference->modifiesStore->getVariablesByProcedure(procedure);
}

std::unordered_set<Procedure> PKBFacadeReader::getModifiesProceduresByVariable(const Variable &variable) const {
    return pkbReference->modifiesStore->getProceduresByVariable(variable);
}

bool PKBFacadeReader::hasProcedureVariableModifiesRelationship(ClauseArgument &arg1, ClauseArgument &arg2) const {
    return pkbReference->modifiesStore->hasProcedureVariableModifiesRelationship(arg1, arg2);
}

std::unordered_set<Variable> PKBFacadeReader::getUsesVariablesByStatement(StmtNum stmt) const {
    return pkbReference->usesStore->getVariablesByStatement(stmt);
}

std::unordered_set<StmtNum> PKBFacadeReader::getUsesStatementsByVariable(const Variable &variable) const {
    return pkbReference->usesStore->getStatementsByVariable(variable);
}

bool PKBFacadeReader::hasStatementVariableUseRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->usesStore->hasStatementVariableUseRelationship(arg1, arg2);
}

std::unordered_set<Variable> PKBFacadeReader::getUsesVariablesByProcedure(const Procedure &procedure) const {
    return pkbReference->usesStore->getVariablesByProcedure(procedure);
}

std::unordered_set<Procedure> PKBFacadeReader::getUsesProceduresByVariable(const Variable &variable) const {
    return pkbReference->usesStore->getProceduresByVariable(variable);
}

bool PKBFacadeReader::hasProcedureVariableUseRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->usesStore->hasProcedureVariableUseRelationship(arg1, arg2);
}

std::unordered_set<StmtNum> PKBFacadeReader::getNexter(StmtNum nextee) {
    return pkbReference->nextStore->getNexter(nextee);
}

std::unordered_set<StmtNum> PKBFacadeReader::getNextee(StmtNum nexter) {
    return pkbReference->nextStore->getNextee(nexter);
}

bool PKBFacadeReader::hasNextRelationship(StmtNum s1, StmtNum s2) {
    return pkbReference->nextStore->hasNextRelationship(s1, s2);
}

bool PKBFacadeReader::hasNextRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->nextStore->hasNextRelationship(arg1, arg2);
}

std::unordered_set<StmtNum> PKBFacadeReader::getNexterStar(StmtNum nextee) {
    return pkbReference->nextStore->getNexterStar(nextee);
}

std::unordered_set<StmtNum> PKBFacadeReader::getNexteeStar(StmtNum nexter) {
    return pkbReference->nextStore->getNexteeStar(nexter);
}

bool PKBFacadeReader::hasNextStarRelationship(StmtNum s1, StmtNum s2) {
    return pkbReference->nextStore->hasNextRelationship(s1, s2);
}

bool PKBFacadeReader::hasNextStarRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->nextStore->hasNextStarRelationship(arg1, arg2);
}

std::unordered_set<Procedure> PKBFacadeReader::getCaller(Procedure callee) {
    return pkbReference->callStore->getCaller(callee);
}

std::unordered_set<Procedure> PKBFacadeReader::getCallee(Procedure caller) {
    return pkbReference->callStore->getCallee(caller);
}

bool PKBFacadeReader::hasCallRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->callStore->hasCallRelationship(arg1, arg2);
}

std::unordered_set<Procedure> PKBFacadeReader::getCallerStar(Procedure callee) {
    return pkbReference->callStore->getCallerStar(callee);
}

std::unordered_set<Procedure> PKBFacadeReader::getCalleeStar(Procedure caller) {
    return pkbReference->callStore->getCalleeStar(caller);
}

bool PKBFacadeReader::hasCallStarRelationship(ClauseArgument &arg1, ClauseArgument &arg2) {
    return pkbReference->callStore->hasCallStarRelationship(arg1, arg2);
}

std::unordered_set<StmtNum> PKBFacadeReader::getStmtNumFromCall(Procedure call) {
    return pkbReference->callStore->getStmtNumFromCall(call);
}

std::optional<Procedure> PKBFacadeReader::getCallFromStmtNum(StmtNum stmtNum) {
    return pkbReference->callStore->getCallFromStmtNum(stmtNum);
}

bool PKBFacadeReader::hasExactPattern(StmtNum stmtNum, std::string arg1, std::string arg2) {
    return pkbReference->assignPatternStore->hasExactPattern(stmtNum, arg1, arg2);
}

bool PKBFacadeReader::hasPartialPattern(StmtNum stmtNum, std::string arg1, std::string arg2) {
    return pkbReference->assignPatternStore->hasPartialPattern(stmtNum, arg1, arg2);
}

bool PKBFacadeReader::hasIfPattern(StmtNum stmtNum, const std::string &arg) {
    return pkbReference->ifPatternStore->hasIfPattern(stmtNum, arg);
}

bool PKBFacadeReader::hasWhilePattern(StmtNum stmtNum, const std::string &arg) {
    return pkbReference->whilePatternStore->hasWhilePattern(stmtNum, arg);
}

std::unordered_map<StmtNum, std::unordered_set<StmtNum>> PKBFacadeReader::getNextStarMap() {
    return pkbReference->nextStore->getNextStarMap();
}
