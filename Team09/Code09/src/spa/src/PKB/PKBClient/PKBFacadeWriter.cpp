#include "PKBFacadeWriter.h"
#include "PKB/PKB.h"

PKBFacadeWriter::PKBFacadeWriter(PKB& pkbReference) {
    this->pkbReference = &pkbReference;
}

void PKBFacadeWriter::setVariables(const std::unordered_set<Variable> &inputVariables) const {
    this->pkbReference->variableStore->addEntities(inputVariables);
}

void PKBFacadeWriter::setConstants(const std::unordered_set<Constant> &inputConstants) const {
    this->pkbReference->constantStore->addEntities(inputConstants);
}

void PKBFacadeWriter::setProcedures(const std::unordered_set<Procedure> &inputProcedures) const {
    this->pkbReference->procedureStore->addEntities(inputProcedures);
}

void PKBFacadeWriter::setStmts(const std::unordered_set<Stmt> &inputStmts) const {
    this->pkbReference->stmtStore->setStatements(inputStmts);
}
