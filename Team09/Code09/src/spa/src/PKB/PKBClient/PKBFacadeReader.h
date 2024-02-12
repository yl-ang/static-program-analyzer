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

    // Contains common entities
    [[nodiscard]] bool containsVariable(const Variable& variable) const;
    [[nodiscard]] bool containsConstant(const Constant& constant) const;
    [[nodiscard]] bool containsProcedure(const Procedure& procedure) const;

    // Getters for Statements
    [[nodiscard]] std::unordered_set<Stmt> getStmts() const;
    [[nodiscard]] Stmt* getStatementByStmtNum(StmtNum stmtNum) const;
    [[nodiscard]] std::vector<Stmt*> getStatementsByType(StatementType type) const;

    // Getters for Follows
    [[nodiscard]] std::optional<StmtNum> getFollower(StmtNum s1) const;
    [[nodiscard]] std::optional<StmtNum> getFollowee(StmtNum s2) const;
    [[nodiscard]] std::unordered_set<StmtNum> getFolloweesStar(StmtNum s) const;
    [[nodiscard]] std::unordered_set<StmtNum> getFollowersStar(StmtNum s) const;
    [[nodiscard]] bool containsFollowRelationship(StmtNum s1, StmtNum s2) const;
    [[nodiscard]] bool containsFollowStarRelationship(StmtNum s1, StmtNum s2) const;


private:
    PKB* pkbReference = nullptr;
};
