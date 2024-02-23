#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"

enum class SuchThatClauseType {
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
    USES,
    MODIFIES
};  // update here

class SuchThatClause : public QueryClause {
private:
    const SuchThatClauseType type;
    const ClauseArgument* firstArg;
    const ClauseArgument* secondArg;

public:
    SuchThatClause(const SuchThatClauseType&, const ClauseArgument*, const ClauseArgument*);
    static SuchThatClauseType determineType(const std::string);  // update here
    ClauseType getType() const override;
    bool equals(const QueryClause& other) const override;
    Table evaluate(const PKBFacadeReader&) override;
};
