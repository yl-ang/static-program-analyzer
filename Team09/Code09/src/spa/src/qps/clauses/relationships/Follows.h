#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Follows : public Relationship {
private:
    ClauseArgument& followee;
    ClauseArgument& follower;

    bool hasNoSynonyms();

    bool hasAtLeastOneWildcard();

    ClauseResult evaluateSynonymInteger(PKBFacadeReader& reader, bool followeeIsSynonym);

    ClauseResult evaluateBothSynonyms(PKBFacadeReader& reader);

public:
    Follows(ClauseArgument& followee, ClauseArgument& follower);

    ClauseResult evaluate(PKBFacadeReader& reader) override;
};
