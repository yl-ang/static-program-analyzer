#pragma once

#include "QueryClause.h"
#include "relationships/Follows.h"
#include "relationships/Relationship.h"

class SuchThatClause : public QueryClause {
private:
    const RelationshipType type;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;

public:
    SuchThatClause(const RelationshipType&, ClauseArgument*, ClauseArgument*);
    static RelationshipType determineRelationshipType(const std::string&);
    ClauseType getType() const override;
    bool equals(const QueryClause& other) const override;
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
};
