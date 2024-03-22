#pragma once

#include "qps/clauses/relationships/Relationship.h"

using StmtSet = std::unordered_set<StmtNum>;

class BaseParent : public Relationship {
protected:
    ClauseArgument& parent;
    ClauseArgument& child;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);
    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    virtual bool hasParentRelationship(PKBFacadeReader&) = 0;
    virtual StmtSet getParents(PKBFacadeReader&, const StmtNum&) = 0;
    virtual StmtSet getChildren(PKBFacadeReader&, const StmtNum&) = 0;

public:
    BaseParent(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
