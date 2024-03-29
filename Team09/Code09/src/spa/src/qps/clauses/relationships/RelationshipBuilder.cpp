#include "RelationshipBuilder.h"

RelationshipType RelationshipBuilder::determineRelationshipType(const std::string& type) {
    auto it = RELATIONSHIP_TYPE_MAP.find(type);
    if (it != RELATIONSHIP_TYPE_MAP.end()) {
        return it->second;
    }
    throw Exception("RelationshipType is not found in valid types: " + type);
}

std::shared_ptr<Relationship> RelationshipBuilder::createRelationship(RelationshipType type,
                                                                      std::shared_ptr<ClauseArgument> firstArg,
                                                                      std::shared_ptr<ClauseArgument> secondArg) {
    switch (type) {
    case (RelationshipType::FOLLOWS):
        return std::make_shared<Follows>(firstArg, secondArg);
    case (RelationshipType::FOLLOWS_STAR):
        return std::make_shared<FollowsStar>(firstArg, secondArg);
    case (RelationshipType::PARENT):
        return std::make_shared<Parent>(firstArg, secondArg);
    case (RelationshipType::PARENT_STAR):
        return std::make_shared<ParentStar>(firstArg, secondArg);
    case (RelationshipType::USES):
        return std::make_shared<Uses>(firstArg, secondArg);
    case (RelationshipType::MODIFIES):
        return std::make_shared<Modifies>(firstArg, secondArg);
    case (RelationshipType::NEXT):
        return std::make_shared<Next>(firstArg, secondArg);
    case (RelationshipType::NEXT_STAR):
        return std::make_shared<NextStar>(firstArg, secondArg);

    case (RelationshipType::CALLS):
        return std::make_shared<Calls>(firstArg, secondArg);
    case (RelationshipType::CALLS_STAR):
        return std::make_shared<CallsStar>(firstArg, secondArg);
    case (RelationshipType::AFFECTS):
        return std::make_shared<Next>(firstArg, secondArg);
    }
    throw Exception("Cannot create relationship");
}
