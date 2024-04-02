#include "WithClause.h"

#include "with/AttributeCollector.h"

WithClause::WithClause(std::shared_ptr<ClauseArgument> l, std::shared_ptr<ClauseArgument> r) : lhs(l), rhs(r) {}

ClauseResult WithClause::evaluate(PKBFacadeReader& pkb) {
    if (isBooleanResult()) {
        return evaluateValueEquality();
    }

    if (lhs->isSynonym() && rhs->isSynonym()) {
        return {{}, {}};
    }

    return evaluateOneSynonym(pkb);
}

ClauseResult WithClause::evaluateValueEquality() const {
    return lhs->getValue() == rhs->getValue();
}

ClauseResult WithClause::evaluateOneSynonym(PKBFacadeReader& pkb) const {
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(lhs->isSynonym() ? lhs : rhs);
    auto other = lhs->isSynonym() ? rhs : lhs;

    // collect all possbile values of synonym
    const std::vector<std::vector<std::string>> synonymValues = SynonymValuesRetriever::retrieve(pkb, {syn});

    // compare attribute values of synonym values collected with non-synonym argument
    std::vector<std::string> validSynonymValues{};
    for (const auto& synonymValue : synonymValues) {
        if (AttributeCollector::collect(pkb, syn, synonymValue[0]) == other->getValue()) {
            validSynonymValues.push_back(synonymValue[0]);
        }
    }

    // We don't want to retain the attributes since we are returning Synonym results.
    return {syn.getWithoutAttribute(), validSynonymValues};
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
