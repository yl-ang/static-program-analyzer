#include "WithClause.h"

WithClause::WithClause(std::shared_ptr<ClauseArgument> l, std::shared_ptr<ClauseArgument> r) : lhs(l), rhs(r) {}

ClauseResult WithClause::evaluate(PKBFacadeReader& pkb) {
    if (isBooleanResult()) {
        return evaluateIntegerEquality();
    }
    return {{}, {}};
}

ClauseResult WithClause::evaluateIntegerEquality() const {
    return lhs->getValue() == rhs->getValue();
}

bool WithClause::equals(const QueryClause& other) const {
    if (const WithClause* ptr = dynamic_cast<const WithClause*>(&other)) {
        return (*lhs == *ptr->lhs && *rhs == *ptr->rhs);
    }
    return false;
}

bool WithClause::isBooleanResult() const {
    return !lhs->isSynonym() && !rhs->isSynonym();
}

bool WithClause::containsSynonym(const Synonym& syn) const {
    return (*lhs == syn || *rhs == syn);
}

std::vector<Synonym> WithClause::getSynonyms() const {
    std::vector<Synonym> synonyms;

    if (lhs->isSynonym()) {
        synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(lhs));
    }
    if (rhs->isSynonym()) {
        synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(rhs));
    }
    return synonyms;
}

bool WithClause::validateArguments(SynonymStore* store) {
    if (lhs->isSynonym()) {
        std::shared_ptr<Synonym> lSyn = std::dynamic_pointer_cast<Synonym>(lhs);
        if (!lSyn->updateType(store)) {
            return false;
        }
    }
    if (rhs->isSynonym()) {
        std::shared_ptr<Synonym> rSyn = std::dynamic_pointer_cast<Synonym>(rhs);
        if (!rSyn->updateType(store)) {
            return false;
        }
    }
    return true;
}
