#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauses/ClauseResult.h"

class CallsStar : public Relationship {
private:
    ClauseArgument& currentProcedure;
    ClauseArgument& nextProcedure;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult evaluate(PKBFacadeReader&);

public:
    CallsStar(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
