#pragma once

#include <string>
#include <unordered_set>

#include "pkb/PKB.h"

class PKBFacadeWriter {
public:
    explicit PKBFacadeWriter(PKB &pkb);
    PKBFacadeWriter() = default;

    // Setters for common entities
    void setVariables(const std::unordered_set<Variable> &inputVariables) const;
    void setConstants(const std::unordered_set<Constant> &inputConstants) const;
    void setProcedures(const std::unordered_set<Procedure> &inputProcedures) const;

    // Setter for statement entity
    void setStmts(const std::unordered_set<Stmt> &inputStmts) const;

    // Setter for FollowsStore
    void setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>> &followsPairs) const;

    // Setter for ParentStore
    void setParentStore(const std::unordered_set<std::pair<StmtNum, StmtNum>> &parentPairs) const;

    // Setter for ModifiesStore
    void setStatementModifiesStore(const std::unordered_set<std::pair<StmtNum, Variable>> &modifiesStatementSet) const;
    void setProcedureModifiesStore(
        const std::unordered_set<std::pair<Procedure, Variable>> &modifiesProcedureSet) const;

    // Setter for UsesStore
    void setStatementUsesStore(const std::unordered_set<std::pair<StmtNum, Variable>> &usesStatementPairs) const;
    void setProcedureUsesStore(const std::unordered_set<std::pair<Procedure, Variable>> &usesProcedurePairs) const;

    // Setter for NextStore
    void setNextStore(const std::unordered_set<std::pair<StmtNum, StmtNum>> &nextPairs) const;

    // Setter for CallStore
    void setCallStore(const std::unordered_set<std::pair<Procedure, Procedure>> &callPairs) const;
    void setCallStmtStore(const std::unordered_set<std::pair<Procedure, StmtNum>> &callStmtPairs) const;

    // Setter for AssignPattern Store
    void setAssignPatternStore(
        const std::function<bool(std::string, std::string)> exactMatchFP,
        const std::function<bool(std::string, std::string)> partialMatchFP,
        const std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> &patterns) const;

    // Setter for IfPattern Store
    void setIfPatternStore(std::unordered_set<std::pair<StmtNum, std::string>> ifPatternPairs) const;

    // Setter for WhilePattern
    void setWhilePatternStore(std::unordered_set<std::pair<StmtNum, std::string>> whilePatternPairs) const;

private:
    PKB *pkbReference = nullptr;
};
