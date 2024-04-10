#pragma once

#include "qps/clauses/relationships/Relationship.h"

using StmtSet = std::unordered_set<StmtNum>;

class BaseFollows : public Relationship {
protected:
    std::shared_ptr<ClauseArgument> followee;
    std::shared_ptr<ClauseArgument> follower;

private:
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&);
    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

    virtual bool hasFollowRelationship(PKBFacadeReader&) = 0;
    virtual StmtSet getFollowers(PKBFacadeReader&, const StmtNum&) = 0;
    virtual StmtSet getFollowees(PKBFacadeReader&, const StmtNum&) = 0;

public:
    BaseFollows(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&) override;
    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
