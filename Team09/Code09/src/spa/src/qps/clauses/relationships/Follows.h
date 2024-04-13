#pragma once

#include "Relationship.h"
#include "baseClasses/BaseFollows.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Follows : public BaseFollows {
public:
    Follows(std::shared_ptr<ClauseArgument> followee, std::shared_ptr<ClauseArgument> follower)
        : BaseFollows(followee, follower) {}

private:
    bool hasFollowRelationship(PKBFacadeReader& reader) override {
        return reader.hasFollowRelationship(*followee, *follower);
    }

    StmtSet getFollowers(PKBFacadeReader& reader, const StmtNum& stmtNum) override {
        std::optional stmtOpt = reader.getFollower(stmtNum);
        return stmtOpt.has_value() ? StmtSet{stmtOpt.value()} : StmtSet{};
    }

    StmtSet getFollowees(PKBFacadeReader& reader, const StmtNum& stmtNum) override {
        std::optional stmtOpt = reader.getFollowee(stmtNum);
        return stmtOpt.has_value() ? StmtSet{stmtOpt.value()} : StmtSet{};
    }
};
