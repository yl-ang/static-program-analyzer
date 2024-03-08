#include "SuchThatClause.h"

#include <iostream>

#include "qps/exceptions/Exception.h"
#include "relationships/Follows.h"
#include "relationships/FollowsStar.h"
#include "relationships/Modifies.h"
#include "relationships/Next.h"
#include "relationships/Parent.h"
#include "relationships/ParentStar.h"
#include "relationships/Uses.h"

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
    switch (type) {
    case (RelationshipType::FOLLOWS):
        return Follows(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::FOLLOWS_STAR):
        return FollowsStar(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::PARENT):
        return Parent(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::PARENT_STAR):
        return ParentStar(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::USES):
        return Uses(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::MODIFIES):
        return Modifies(firstArg, secondArg).evaluate(reader);

    case (RelationshipType::NEXT):
        return Next(firstArg, secondArg).evaluate(reader);
    }

    return {false};
}
