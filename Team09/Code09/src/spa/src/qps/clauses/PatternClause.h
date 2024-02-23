#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ExpressionSpec.h"

class PatternClause : public QueryClause {
private:
    const ClauseArgument* assignSynonym;
    const ClauseArgument* firstArg;
    const ClauseArgument* secondArg;

public:
    PatternClause(const ClauseArgument*, const ClauseArgument*, const ClauseArgument*);
    ClauseType getType();
    bool equals(const QueryClause&);
    // Table evaluate(const PKBFacadeReader&) override;
};
