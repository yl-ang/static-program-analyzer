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
    bool bothAreWildcards();
    bool hasAtLeastOneWildcard();

    ClauseResult evaluateSynonymInteger(PKBFacadeReader&, bool);
    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&, bool);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

public:
    Follows(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
};
