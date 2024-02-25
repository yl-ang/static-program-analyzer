#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Synonym.h"

class PatternClause : public QueryClause {
private:
    ClauseArgument& assignSynonym;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;

    ClauseResult evaluateSynonym(PKBFacadeReader&);
    ClauseResult evaluateOthers(PKBFacadeReader&);

public:
    PatternClause(ClauseArgument* assign, ClauseArgument* f, ClauseArgument* s);
    ClauseType getType() const override;
    bool equals(const QueryClause&) const override;
    void print() const;
    ClauseResult evaluate(PKBFacadeReader&) override;
};
