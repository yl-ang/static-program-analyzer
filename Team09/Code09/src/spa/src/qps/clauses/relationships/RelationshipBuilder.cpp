#include "RelationshipBuilder.h"

RelationshipType RelationshipBuilder::determineRelationshipType(const std::string& type) {
    auto it = RELATIONSHIP_TYPE_MAP.find(type);
    if (it != RELATIONSHIP_TYPE_MAP.end()) {
        return it->second;
    }
    throw Exception("RelationshipType is not found in valid types: " + type);
}

std::unique_ptr<Relationship> RelationshipBuilder::createRelationship(RelationshipType type, ClauseArgument& firstArg,
                                                                      ClauseArgument& secondArg) {
    switch (type) {
    case (RelationshipType::FOLLOWS):
        return std::make_unique<Follows>(firstArg, secondArg);
    case (RelationshipType::FOLLOWS_STAR):
        return std::make_unique<FollowsStar>(firstArg, secondArg);
    case (RelationshipType::PARENT):
        return std::make_unique<Parent>(firstArg, secondArg);
    case (RelationshipType::PARENT_STAR):
        return std::make_unique<ParentStar>(firstArg, secondArg);
    case (RelationshipType::USES):
        return std::make_unique<Uses>(firstArg, secondArg);
    case (RelationshipType::MODIFIES):
        return std::make_unique<Modifies>(firstArg, secondArg);
    case (RelationshipType::NEXT):
        return std::make_unique<Next>(firstArg, secondArg);
    case (RelationshipType::NEXT_STAR):
        return std::make_unique<NextStar>(firstArg, secondArg);

    case (RelationshipType::CALLS):
        return std::make_unique<Next>(firstArg, secondArg);
    case (RelationshipType::CALLS_STAR):
        return std::make_unique<NextStar>(firstArg, secondArg);
    case (RelationshipType::AFFECTS):
        return std::make_unique<Next>(firstArg, secondArg);
    }
    throw Exception("Cannot create relationship");
}
