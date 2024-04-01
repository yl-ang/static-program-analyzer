#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Affects : public Relationship {
private:
    std::shared_ptr<ClauseArgument> affector;
    std::shared_ptr<ClauseArgument> affected;

public:
    Affects(std::shared_ptr<ClauseArgument> affector, std::shared_ptr<ClauseArgument> affected)
        : affector(affector), affected(affected) {}

    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
