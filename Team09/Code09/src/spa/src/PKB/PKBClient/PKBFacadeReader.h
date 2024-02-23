#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeReader {
public:
    PKBFacadeReader(PKB& pkbReference);
    PKBFacadeReader() = default;

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
    [[nodiscard]] std::unordered_set<Stmt*> getStatementsByType(StatementType type) const;

    // Getters for FollowsStore
    std::optional<StmtNum> getFollower(StmtNum s1);
    std::optional<StmtNum> getFollowee(StmtNum s2);
    std::unordered_set<StmtNum> getFollowersStar(StmtNum s);
    std::unordered_set<StmtNum> getFolloweesStar(StmtNum s);
    bool hasFollowRelationship(StmtNum s1, StmtNum s2);
    bool hasFollowStarRelationship(StmtNum s1, StmtNum s2);
    bool hasFollowRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    bool hasFollowStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for ParentStore

    // Getters for ModifiesStore

    // Getters for UsesStore

    // Getters for PatternStore

private:
    PKB* pkbReference = nullptr;
};
