#include "PKBFacadeReader.h"
#include "PKB/PKB.h"

PKBFacadeReader::PKBFacadeReader(PKB& pkbReference) {
    this->pkbReference = &pkbReference;
}

std::unordered_set<std::string> PKBFacadeReader::getVariables() const {
    return pkbReference->variablesStore->getVariablesStore();
}

std::unordered_set<int> PKBFacadeReader::getStmts() const {
    return {};
}

std::unordered_set<int> PKBFacadeReader::getConstants() const {
    return {};
}

std::unordered_set<std::string> PKBFacadeReader::getProcedures() const {
    return {};
}
