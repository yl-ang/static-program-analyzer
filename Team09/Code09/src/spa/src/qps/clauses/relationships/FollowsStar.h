#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseFollows.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class FollowsStar : public BaseFollows {
public:
    FollowsStar(std::shared_ptr<ClauseArgument> followee, std::shared_ptr<ClauseArgument> follower)
        : BaseFollows(followee, follower) {}

private:
    bool hasFollowRelationship(PKBFacadeReader& reader) override {
        return reader.hasFollowStarRelationship(*followee, *follower);
    }

    StmtSet getFollowers(PKBFacadeReader& reader, const StmtNum& stmtNum) override {
        return reader.getFollowersStar(stmtNum);
    }

    StmtSet getFollowees(PKBFacadeReader& reader, const StmtNum& stmtNum) override {
        return reader.getFolloweesStar(stmtNum);
    }
};
