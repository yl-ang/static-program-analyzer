#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"

enum class SuchThatClauseType {
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
};  // update here

class SuchThatClause : public QueryClause {
private:
    SuchThatClauseType type;
    Synonym firstArg;
    Synonym secondArg;

public:
    SuchThatClause(const SuchThatClauseType&, const Synonym&, const Synonym&);
    static SuchThatClauseType determineType(const std::string);  // update here
    ClauseType getType() const override;
    bool equals(const QueryClause& other) const override;
    Table evaluate(const PKBFacadeReader&) override;
};
