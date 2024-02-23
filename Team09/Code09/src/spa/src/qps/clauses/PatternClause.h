#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ExpressionSpec.h"

class PatternClause : public QueryClause {
private:
    const Synonym* assignSynonym;
    const ClauseArgument* firstArg;
    const ExpressionSpec* secondArg;

public:
    PatternClause(const Synonym*, const ClauseArgument*, const ExpressionSpec*);
    ClauseType getType() const override;
    bool equals(const QueryClause&) const override;
    // Table evaluate(const PKBFacadeReader&) override;
};
