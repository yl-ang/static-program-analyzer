#pragma once

#include <unordered_set>
#include <string>
#include "PKB/PKB.h"

class PKBFacadeReader {
public:
    explicit PKBFacadeReader(PKB &pkbReference);

    // Getters for common entities
    [[nodiscard]] std::unordered_set<Variable> getVariables() const;
    [[nodiscard]] std::unordered_set<Constant> getConstants() const;
    [[nodiscard]] std::unordered_set<Procedure> getProcedures() const;

    // Getters for Statements
    [[nodiscard]] std::unordered_set<Stmt> getStmts() const;
    [[nodiscard]] Stmt* getStatementByStmtNum(StmtNum stmtNum) const;
    [[nodiscard]] std::vector<Stmt*> getStatementsByType(StatementType type) const;

private:
    PKB* pkbReference = nullptr;
};
