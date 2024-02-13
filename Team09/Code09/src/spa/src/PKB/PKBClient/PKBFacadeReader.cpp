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

std::vector<Stmt*> PKBFacadeReader::getStatementsByType(
    StatementType type) const {
  return pkbReference->stmtStore->getStatementsByType(type);
}
