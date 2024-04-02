#pragma once

#include "QueryClause.h"
#include "relationships/Relationship.h"
#include "relationships/RelationshipBuilder.h"

class SuchThatClause : public QueryClause {
private:
    const RelationshipType type;
    std::shared_ptr<ClauseArgument> firstArg;
    std::shared_ptr<ClauseArgument> secondArg;

    std::optional<std::shared_ptr<Relationship>> relationship;

protected:
    ClauseResult evaluate(PKBFacadeReader&) override;

public:
    SuchThatClause(const RelationshipType&, std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);
    bool equals(const QueryClause& other) const override;
    bool isBooleanResult() const override;
    bool containsSynonym(const Synonym&) const override;
    std::vector<Synonym> getSynonyms() const override;
    std::shared_ptr<Relationship> getRelationship();
    bool validateArguments(SynonymStore* store) override;
};
