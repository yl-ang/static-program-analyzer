#pragma once

#include "qps/clauses/relationships/Relationship.h"

using StmtSet = std::unordered_set<StmtNum>;

class BaseNext : public Relationship {
protected:
    std::shared_ptr<ClauseArgument> currentStmt;
    std::shared_ptr<ClauseArgument> nextStmt;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymInteger(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);

    virtual ClauseResult evaluateBothSynonyms(PKBFacadeReader&, EvaluationDb&) = 0;
    virtual bool hasNextRelationship(PKBFacadeReader&) = 0;
    virtual bool hasNextRelationship(PKBFacadeReader&, const StmtNum&, const StmtNum&) = 0;
    virtual StmtSet getNexters(PKBFacadeReader&, const StmtNum&) = 0;
    virtual StmtSet getNextees(PKBFacadeReader&, const StmtNum&) = 0;

public:
    BaseNext(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
