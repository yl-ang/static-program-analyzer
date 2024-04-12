#include "SuchThatClause.h"

SuchThatClause::SuchThatClause(const RelationshipType& t, std::shared_ptr<ClauseArgument> f,
                               std::shared_ptr<ClauseArgument> s)
    : type(t), firstArg(f), secondArg(s) {}

bool SuchThatClause::equals(const QueryClause& other) const {
    if (const SuchThatClause* ptr = dynamic_cast<const SuchThatClause*>(&other)) {
        return type == ptr->type && (*firstArg) == *(ptr->firstArg) && (*secondArg) == *(ptr->secondArg);
    }
    return false;
}

bool SuchThatClause::isBooleanResult() const {
    return !firstArg->isSynonym() && !secondArg->isSynonym();
}

bool SuchThatClause::containsSynonym(const Synonym& s) const {
    return (firstArg->isSynonym() && *std::dynamic_pointer_cast<Synonym>(firstArg) == s) ||
           (secondArg->isSynonym() && *std::dynamic_pointer_cast<Synonym>(secondArg) == s);
}

std::vector<Synonym> SuchThatClause::getSynonyms() const {
    std::vector<Synonym> synonyms;
    if (firstArg->isSynonym()) {
        synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(firstArg));
    }
    if (secondArg->isSynonym()) {
        synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(secondArg));
    }
    return synonyms;
}

ClauseResult SuchThatClause::evaluate(PKBFacadeReader& reader) {
    auto relationship = getRelationship();
    return relationship->evaluate(reader);
}

ClauseResult SuchThatClause::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    auto relationship = getRelationship();
    return relationship->evaluate(reader, evalDb);
}

std::shared_ptr<Relationship> SuchThatClause::getRelationship() {
    if (!relationship.has_value()) {
        relationship = RelationshipBuilder::createRelationship(type, firstArg, secondArg);
    }
    return relationship.value();
}

bool SuchThatClause::validateArguments(SynonymStore* store) {
    if (firstArg->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(firstArg);
        if (!first->updateType(store)) {
            return false;
        }
    }
    if (secondArg->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(secondArg);
        if (!second->updateType(store)) {
            return false;
        }
    }
    // Check semantic based on if there is synonyms and Relationship types
    // Build relationship, check semantic in relationship
    auto relationship = getRelationship();
    return relationship->validateArguments();
}
