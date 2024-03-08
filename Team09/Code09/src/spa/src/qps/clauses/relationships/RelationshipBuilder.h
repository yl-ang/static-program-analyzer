#pragma once

#include <unordered_map>

#include "pkb/Utils/DataTypes.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/ClauseResult.h"

#include "qps/exceptions/Exception.h"
#include "Relationship.h"
#include "Follows.h"
#include "FollowsStar.h"
#include "Modifies.h"
#include "Next.h"
#include "Parent.h"
#include "ParentStar.h"
#include "Uses.h"

static const std::unordered_map<std::string, RelationshipType> RELATIONSHIP_TYPE_MAP = {
    {"Follows", RelationshipType::FOLLOWS}, {"Follows*", RelationshipType::FOLLOWS_STAR},
    {"Parent", RelationshipType::PARENT},   {"Parent*", RelationshipType::PARENT_STAR},
    {"Uses", RelationshipType::USES},       {"Modifies", RelationshipType::MODIFIES},
    {"Next", RelationshipType::NEXT}};  // update here

class RelationshipBuilder {
public:
    static RelationshipType determineRelationshipType(const std::string& type);

    static std::unique_ptr<Relationship> createRelationship(RelationshipType type,
                                                            ClauseArgument& firstArg,
                                                            ClauseArgument& secondArg);
};
