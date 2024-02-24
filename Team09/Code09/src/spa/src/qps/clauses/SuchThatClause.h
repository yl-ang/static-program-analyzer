#pragma once

#include "QueryClause.h"
#include "qps/clauseArguments/Synonym.h"

enum class RelationshipType { FOLLOWS, FOLLOWS_STAR, PARENT, PARENT_STAR, USES, MODIFIES };  // update here

static const std::unordered_map<std::string, RelationshipType> relationshipTypeMap = {
    {"Follows", RelationshipType::FOLLOWS},
    {"Follows*", RelationshipType::FOLLOWS_STAR},
    {"Parent", RelationshipType::PARENT},
    {"Parent*", RelationshipType::PARENT_STAR},
};  // update here

class SuchThatClause : public QueryClause {
private:
    const RelationshipType type;
    const ClauseArgument* firstArg;
    const ClauseArgument* secondArg;

public:
    SuchThatClause(const RelationshipType&, ClauseArgument*, ClauseArgument*);
    static RelationshipType determineRelationshipType(const std::string&);
    ClauseType getType();
    bool equals(const QueryClause& other);
    // ClauseResult evaluate(PKBFacadeReader&) override;
};
