#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeReader {
public:
    PKBFacadeReader(PKB& pkbReference);
    PKBFacadeReader() = default;

    // Getters for common entities4
    [[nodiscard]] std::unordered_set<Variable> getVariables() const;
    [[nodiscard]] std::unordered_set<Constant> getConstants() const;
    [[nodiscard]] std::unordered_set<Procedure> getProcedures() const;

    // Contains common entities
    [[nodiscard]] bool containsVariable(const Variable& variable) const;
    [[nodiscard]] bool containsConstant(const Constant& constant) const;
    [[nodiscard]] bool containsProcedure(const Procedure& procedure) const;

    // Getters for Statements
    [[nodiscard]] std::unordered_set<Stmt> getStmts() const;
    [[nodiscard]] Stmt* getStatementByStmtNum(StmtNum stmtNum) const;
    [[nodiscard]] std::unordered_set<Stmt*> getStatementsByType(
        StatementType type) const;

private:
    PKB* pkbReference = nullptr;
};
