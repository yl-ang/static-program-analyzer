#pragma once

#include <unordered_map>

#include "Calls.h"
#include "CallsStar.h"
#include "Follows.h"
#include "FollowsStar.h"
#include "Modifies.h"
#include "Next.h"
#include "NextStar.h"
#include "PKB/Utils/DataTypes.h"
#include "Parent.h"
#include "ParentStar.h"
#include "Relationship.h"
#include "Uses.h"
#include "qps/QPSConstants.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/ClauseResult.h"
#include "qps/exceptions/Exception.h"

static const std::unordered_map<std::string, RelationshipType> RELATIONSHIP_TYPE_MAP = {
    {QPSConstants::FOLLOWS, RelationshipType::FOLLOWS}, {QPSConstants::FOLLOWS_STAR, RelationshipType::FOLLOWS_STAR},
    {QPSConstants::PARENT, RelationshipType::PARENT},   {QPSConstants::PARENT_STAR, RelationshipType::PARENT_STAR},
    {QPSConstants::USES, RelationshipType::USES},       {QPSConstants::MODIFIES, RelationshipType::MODIFIES},
    {QPSConstants::NEXT, RelationshipType::NEXT},       {QPSConstants::NEXT_STAR, RelationshipType::NEXT_STAR},
    {QPSConstants::CALLS, RelationshipType::CALLS},     {QPSConstants::CALLS_STAR, RelationshipType::CALLS_STAR},
    {QPSConstants::AFFECTS, RelationshipType::AFFECTS}};  // update here

class RelationshipBuilder {
public:
    static RelationshipType determineRelationshipType(const std::string& type);

    static std::shared_ptr<Relationship> createRelationship(RelationshipType type,
                                                            std::shared_ptr<ClauseArgument> firstArg,
                                                            std::shared_ptr<ClauseArgument> secondArg);
};
