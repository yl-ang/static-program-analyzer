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

bool SuchThatClause::isBooleanResult() const {
    return !firstArg.isSynonym() && !secondArg.isSynonym();
}

bool SuchThatClause::containsSynonym(const Synonym& s) const {
    return firstArg == s || secondArg == s;
}

std::vector<Synonym> SuchThatClause::getSynonyms() const {
    std::vector<Synonym> synonyms;
    if (firstArg.isSynonym()) {
        synonyms.push_back(*dynamic_cast<Synonym*>(&firstArg));
    }
    if (secondArg.isSynonym()) {
        synonyms.push_back(*dynamic_cast<Synonym*>(&secondArg));
    }
    return synonyms;
}

ClauseResult SuchThatClause::evaluate(PKBFacadeReader& reader) {
    auto relationship = RelationshipBuilder::createRelationship(type, firstArg, secondArg);
    return relationship->evaluate(reader);
}
