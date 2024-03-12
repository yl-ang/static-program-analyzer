#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Affects : public Relationship {
private:
    ClauseArgument& affector;
    ClauseArgument& affected;

public:
    Affects(ClauseArgument& affector, ClauseArgument& affected) : affector(affector), affected(affected) {}

    ClauseResult evaluate(PKBFacadeReader&) override;
};
