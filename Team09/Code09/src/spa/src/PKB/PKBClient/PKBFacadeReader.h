#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeReader {
public:
    PKBFacadeReader(PKB& pkbReference);
    PKBFacadeReader() = default;

    // Common Entities (Variables, Constants, Procedures)
    [[nodiscard]] std::unordered_set<Variable> getVariables() const;
    [[nodiscard]] std::unordered_set<Constant> getConstants() const;
    [[nodiscard]] std::unordered_set<Procedure> getProcedures() const;
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
    std::optional<StmtNum> getParent(StmtNum child);
    std::unordered_set<StmtNum> getChildren(StmtNum parent);
    std::unordered_set<StmtNum> getChildrenStar(StmtNum parent);
    std::unordered_set<StmtNum> getParentsStar(StmtNum child);
    bool hasParentRelationship(StmtNum parent, StmtNum child);
    bool hasParentStarRelationship(StmtNum parent, StmtNum child);
    bool hasParentRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    bool hasParentStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for ModifiesStore
    std::unordered_set<Variable> getModifiesVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getModifiesStatementsByVariable(const Variable& variable) const;
    bool hasStatementVariableModifiesRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;

    // Getters for UsesStore
    std::unordered_set<Variable> getUsesVariablesByStatement(StmtNum stmt) const;
    std::unordered_set<StmtNum> getUsesStatementsByVariable(const Variable& variable) const;
    bool hasStatementVariableUseRelationship(StmtNum stmt, const Variable& variable) const;
    bool hasStatementVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for PatternStore
    bool hasPattern(StmtNum stmtNum, std::string lhs, std::string rhs);
    bool hasPattern(StmtNum stmtNum, ClauseArgument& arg1, ClauseArgument& arg2);

private:
    PKB* pkbReference = nullptr;
};
