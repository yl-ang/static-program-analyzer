#pragma once

#include "../parser/QueryEntity.h"
#include "QueryClause.h"

class PatternClause : public QueryClause {
private:
    QueryEntity assignSynonym;
    QueryEntity firstArg;
    QueryEntity secondArg;

public:
    PatternClause(const QueryEntity&, const QueryEntity&, const QueryEntity&);
    ClauseType getType() const override;
    bool equals(const QueryClause&) const override;
    Table evaluate(const PKBFacadeReader&) override;
}
