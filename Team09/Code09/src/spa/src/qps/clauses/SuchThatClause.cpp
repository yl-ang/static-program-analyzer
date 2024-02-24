#include "SuchThatClause.h"

#include <iostream>

#include "qps/clauseArguments/Integer.h"
#include "qps/exceptions/Exception.h"
#include "relationships/Follows.h"
#include "relationships/FollowsStar.h"
#include "relationships/Parent.h"

namespace {
static const std::unordered_map<std::string, RelationshipType> RELATIONSHIP_TYPE_MAP = {
    {"Follows", RelationshipType::FOLLOWS}, {"Follows*", RelationshipType::FOLLOWS_STAR},
    {"Parent", RelationshipType::PARENT},   {"Parent*", RelationshipType::PARENT_STAR},
    {"Uses", RelationshipType::USES},       {"Modifies", RelationshipType::MODIFIES}};  // update here
}

SuchThatClause::SuchThatClause(const RelationshipType& t, ClauseArgument* f, ClauseArgument* s)
    : type(t), firstArg(*f), secondArg(*s) {}

ClauseType SuchThatClause::getType() const {
    return ClauseType::SUCH_THAT;
}
bool SuchThatClause::equals(const QueryClause& other) const {
    if (const SuchThatClause* ptr = dynamic_cast<const SuchThatClause*>(&other)) {
        return type == ptr->type && (firstArg) == (ptr->firstArg) && (secondArg) == (ptr->secondArg);
    }
    return false;
}

RelationshipType SuchThatClause::determineRelationshipType(const std::string& type) {
    auto it = RELATIONSHIP_TYPE_MAP.find(type);
    if (it != RELATIONSHIP_TYPE_MAP.end()) {
        return it->second;
    } else {
        throw Exception("suchThatClauseType is not found in valid types: " + type);
    }
}

ClauseResult SuchThatClause::evaluate(PKBFacadeReader& reader) {
    switch (type) {
    case (RelationshipType::FOLLOWS):
        return Follows(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::FOLLOWS_STAR):
        return FollowsStar(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::PARENT):
        return Parent(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::PARENT_STAR):
        return {false};
    default:
        return {false};
    }
}
