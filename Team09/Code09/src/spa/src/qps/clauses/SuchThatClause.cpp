#include "SuchThatClause.h"

#include <iostream>

#include "qps/exceptions/Exception.h"

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

ClauseResult SuchThatClause::evaluate(PKBFacadeReader& reader) {
    auto relationship = RelationshipBuilder::createRelationship(type, firstArg, secondArg);
    return relationship->evaluate(reader);
}
