#pragma once

#include "../parser/QueryEntity.h"
#include "QueryClause.h"

enum class SuchThatClauseType {
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
};

class SuchThatClause : public QueryClause {
private:
    SuchThatClauseType type;
    QueryEntity firstArg;
    QueryEntity secondArg;

public:
    SuchThatClause(const SuchThatClauseType&, const QueryEntity&,
                   const QueryEntity&);
    ClauseType getType() const override;
    bool equals(const QueryClause& other) const override;
    Table evaluate(const PKBFacadeReader&) override;
};
