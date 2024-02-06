#pragma once

#include <unordered_set>
#include <string>
#include "PKB/PKB.h"
#include "PKB/Utils/HashSpecialization.h"

class PKBFacadeWriter {
public:
    explicit PKBFacadeWriter(PKB &pkb);

    // Setters for entities
    void setVariables(std::unordered_set<std::string> &variables) const;
    void setStmts(const std::unordered_set<int> &stmts) const;
    void setConstants(const std::unordered_set<int> &constants) const;
    void setProcedures(const std::unordered_set<std::string> &procedures) const;

    // setters for design abstractions
    //    void setFollowRelation() const;
    //    void setFollowStarRelation() const;
    //    void setParentRelation() const;
    //    void setParentStarRelation() const;
    //    void setUsesRelation() const;
    //    void setUsesStarRelation() const;
    //    void setModifiesStarRelation() const;

private:
    PKB *pkbReference = nullptr;
};
