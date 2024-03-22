#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "baseClasses/BaseFollows.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Follows : public BaseFollows {
private:
    bool hasFollowRelationship(PKBFacadeReader&) override;
    StmtSet getFollowers(PKBFacadeReader&, const StmtNum&) override;
    StmtSet getFollowees(PKBFacadeReader&, const StmtNum&) override;

public:
    Follows(ClauseArgument&, ClauseArgument&);
};
