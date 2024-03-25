#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeReader {
public:
    explicit PKBFacadeReader(PKB& pkbReference);
    PKBFacadeReader() = default;

    // Common Entities (Variables, Constants, Procedures)
    [[nodiscard]] std::unordered_set<Variable> getVariables() const;
    [[nodiscard]] std::unordered_set<Constant> getConstants() const;
    [[nodiscard]] std::unordered_set<Procedure> getProcedures() const;

    // Getters for Statements
    [[nodiscard]] std::unordered_set<Stmt> getStmts() const;
    [[nodiscard]] std::optional<Stmt> getStatementByStmtNum(StmtNum stmtNum) const;
    [[nodiscard]] std::unordered_set<Stmt> getStatementsByType(StatementType type) const;

    // Getters for FollowsStore
    std::optional<StmtNum> getFollower(StmtNum s1);
    std::optional<StmtNum> getFollowee(StmtNum s2);
    std::unordered_set<StmtNum> getFollowersStar(StmtNum s);
    std::unordered_set<StmtNum> getFolloweesStar(StmtNum s);
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

    // Getters for ModifiesStore (Statements)
    [[nodiscard]] std::unordered_set<Variable> getModifiesVariablesByStatement(StmtNum stmt) const;
    [[nodiscard]] std::unordered_set<StmtNum> getModifiesStatementsByVariable(const Variable& variable) const;
    [[nodiscard]] bool hasStatementVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;

    // Getters for ModifiesStore (Procedures)
    [[nodiscard]] std::unordered_set<Variable> getModifiesVariablesByProcedure(const Procedure& procedure) const;
    [[nodiscard]] std::unordered_set<Procedure> getModifiesProceduresByVariable(const Variable& variable) const;
    [[nodiscard]] bool hasProcedureVariableModifiesRelationship(ClauseArgument& arg1, ClauseArgument& arg2) const;

    // Getters for UsesStore (Statements)
    [[nodiscard]] std::unordered_set<Variable> getUsesVariablesByStatement(StmtNum stmt) const;
    [[nodiscard]] std::unordered_set<StmtNum> getUsesStatementsByVariable(const Variable& variable) const;
    [[nodiscard]] bool hasStatementVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for UsesStore (Procedures)
    [[nodiscard]] std::unordered_set<Variable> getUsesVariablesByProcedure(const Procedure& procedure) const;
    [[nodiscard]] std::unordered_set<Procedure> getUsesProceduresByVariable(const Variable& variable) const;
    [[nodiscard]] bool hasProcedureVariableUseRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for NextStore
    [[nodiscard]] std::unordered_set<StmtNum> getNexter(StmtNum nextee);
    [[nodiscard]] std::unordered_set<StmtNum> getNextee(StmtNum nexter);
    [[nodiscard]] std::unordered_set<StmtNum> getNexterStar(StmtNum nextee);
    [[nodiscard]] std::unordered_set<StmtNum> getNexteeStar(StmtNum nexter);
    [[nodiscard]] bool hasNextRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    [[nodiscard]] bool hasNextStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for CallStore (Call and Call relationships)
    [[nodiscard]] std::unordered_set<Procedure> getCaller(Procedure callee);
    [[nodiscard]] std::unordered_set<Procedure> getCallee(Procedure caller);
    [[nodiscard]] std::unordered_set<Procedure> getCallerStar(Procedure callee);
    [[nodiscard]] std::unordered_set<Procedure> getCalleeStar(Procedure caller);
    [[nodiscard]] bool hasCallRelationship(ClauseArgument& arg1, ClauseArgument& arg2);
    [[nodiscard]] bool hasCallStarRelationship(ClauseArgument& arg1, ClauseArgument& arg2);

    // Getters for CallStore (Call and StmtNumber)
    [[nodiscard]] std::unordered_set<StmtNum> getStmtNumFromCall(Procedure call);
    [[nodiscard]] std::optional<Procedure> getCallFromStmtNum(StmtNum stmtNum);

    // Getters for AssignPatternStore
    [[nodiscard]] bool hasExactPattern(StmtNum stmtNum, std::string arg1, std::string arg2);
    [[nodiscard]] bool hasPartialPattern(StmtNum stmtNum, std::string arg1, std::string arg2);

    // Getters for IfPatternStore
    [[nodiscard]] bool hasIfPattern(StmtNum stmtNum, const std::string& arg);

    // Getters for WhilePatternStore
    [[nodiscard]] bool hasWhilePattern(StmtNum stmtNum, const std::string& arg);

private:
    PKB* pkbReference = nullptr;
};
