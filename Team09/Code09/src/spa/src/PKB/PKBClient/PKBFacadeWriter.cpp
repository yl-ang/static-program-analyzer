#include "PKBFacadeWriter.h"
#include "PKB/PKB.h"

PKBFacadeWriter::PKBFacadeWriter(PKB& pkbReference) {
    this->pkbReference = &pkbReference;
}

void PKBFacadeWriter::setVariables(std::unordered_set<std::string> &variables) const {
    this->pkbReference->variablesStore->setVariables(variables);
}

void PKBFacadeWriter::setStmts(const std::unordered_set<int> &stmts) const {}

void PKBFacadeWriter::setConstants(const std::unordered_set<int> &constants) const {}

void PKBFacadeWriter::setProcedures(const std::unordered_set<std::string> &procedures) const {}
