#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"

class PatternClause : public QueryClause {
private:
    Synonym assignSynonym;
    Synonym firstArg;
    Synonym secondArg;

public:
    PatternClause(const Synonym&, const Synonym&, const Synonym&);
    ClauseType getType() const override;
    bool equals(const QueryClause&) const override;
    Table evaluate(const PKBFacadeReader&) override;
};
