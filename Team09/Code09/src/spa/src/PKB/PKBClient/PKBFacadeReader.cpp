#include "PKBFacadeReader.h"
#include "PKB/PKB.h"

PKBFacadeReader::PKBFacadeReader(PKB& pkbReference) {
    this->pkbReference = &pkbReference;
}

std::unordered_set<std::string> PKBFacadeReader::getVariables() const {
    return pkbReference->variablesStore->getAllEntities();
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

Stmt *PKBFacadeReader::getStatementByStmtNum(StmtNum stmtNum) const {
    return pkbReference->stmtStore->getStatementByStmtNum(stmtNum);
}

std::vector<Stmt *> PKBFacadeReader::getStatementsByType(StatementType type) const {
    return pkbReference->stmtStore->getStatementsByType(type);
}


