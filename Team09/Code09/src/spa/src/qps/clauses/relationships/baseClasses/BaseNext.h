#pragma once

#include "qps/clauses/relationships/Relationship.h"

using StmtSet = std::unordered_set<StmtNum>;

class BaseNext : public Relationship {
protected:
    ClauseArgument& currentStmt;
    ClauseArgument& nextStmt;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    virtual bool hasNextRelationship(PKBFacadeReader&) = 0;
    virtual StmtSet getNexters(PKBFacadeReader&, const StmtNum&) = 0;
    virtual StmtSet getNextees(PKBFacadeReader&, const StmtNum&) = 0;

public:
    BaseNext(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
