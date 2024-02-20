#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKB.h"

class PKBFacadeWriter {
public:
    explicit PKBFacadeWriter(PKB &pkb);
    PKBFacadeWriter() = default;

    // Setters for common entities
    void setVariables(const std::unordered_set<Variable> &inputVariables) const;
    void setConstants(const std::unordered_set<Constant> &inputConstants) const;
    void setProcedures(
        const std::unordered_set<Procedure> &inputProcedures) const;

    // Setter for statement entity
    void setStmts(const std::unordered_set<Stmt> &inputStmts) const;

    // Setter for FollowsStore
    void setFollowsStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>
                             &followsPairs) const;

    // Setter for ParentStore
    void setParentStore(const std::unordered_set<std::pair<StmtNum, StmtNum>>
                            &parentPairs) const;

    // Setter for ModifiesStore
    void setModifiesStore(const std::unordered_set<std::pair<StmtNum, Variable>>
                              &modifiesPairs) const;

    // Setter for UsesStore
    void setUsesStore(const std::unordered_set<std::pair<StmtNum, Variable>>
                          &usesPairs) const;

private:
    PKB *pkbReference = nullptr;
};
