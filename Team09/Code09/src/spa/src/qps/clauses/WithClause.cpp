#include "WithClause.h"

#include "with/AttributeCollector.h"

WithClause::WithClause(std::shared_ptr<ClauseArgument> l, std::shared_ptr<ClauseArgument> r) : lhs(l), rhs(r) {}

ClauseResult WithClause::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    if (isBooleanResult()) {
        return evaluateValueEquality();
    }

    if (lhs->isSynonym() && rhs->isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    return evaluateOneSynonym(reader);
}

ClauseResult WithClause::evaluateValueEquality() const {
    return lhs->getValue() == rhs->getValue();
}

ClauseResult WithClause::evaluateOneSynonym(PKBFacadeReader& pkb) const {
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(lhs->isSynonym() ? lhs : rhs);
    auto other = lhs->isSynonym() ? rhs : lhs;

    // collect all possbile values of synonym
    std::vector<std::string> synonymValues = SynonymValuesRetriever::retrieve(pkb, syn);

    // compare attribute values of synonym values collected with non-synonym argument
    std::vector<std::string> validSynonymValues{};
    for (const auto& synonymValue : synonymValues) {
        if (AttributeCollector::collect(pkb, syn, synonymValue) == other->getValue()) {
            validSynonymValues.push_back(synonymValue);
        }
    }

    // We don't want to retain the attributes since we are returning Synonym results.
    return {syn.getWithoutAttribute(), validSynonymValues};
}

ClauseResult WithClause::evaluateBothSynonyms(PKBFacadeReader& pkb) const {
    Synonym lhsSyn = *std::dynamic_pointer_cast<Synonym>(lhs);
    Synonym rhsSyn = *std::dynamic_pointer_cast<Synonym>(rhs);

    // collect all possbile values of both synonyms
    std::vector<std::string> lhsValues = SynonymValuesRetriever::retrieve(pkb, lhsSyn);
    std::vector<std::string> rhsValues = SynonymValuesRetriever::retrieve(pkb, rhsSyn);

    std::unordered_map<std::string, std::vector<std::string>> rhsAttributesMap{};
    for (std::string rhsValue : rhsValues) {
        rhsAttributesMap[AttributeCollector::collect(pkb, rhsSyn, rhsValue)].push_back(rhsValue);
    }

    std::vector<std::string> validLhsValues{};
    std::vector<std::string> validRhsValues{};
    for (std::string lhsValue : lhsValues) {
        auto lhsAttribute = AttributeCollector::collect(pkb, lhsSyn, lhsValue);

        if (rhsAttributesMap.find(lhsAttribute) == rhsAttributesMap.end()) {
            continue;
        }

        for (std::string rhsValue : rhsAttributesMap[lhsAttribute]) {
            validLhsValues.push_back(lhsValue);
            validRhsValues.push_back(rhsValue);
        }
    }

    return {{lhsSyn.getWithoutAttribute(), rhsSyn.getWithoutAttribute()}, {validLhsValues, validRhsValues}};
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

WithType WithClause::determineWithType(std::shared_ptr<ClauseArgument> compAttr) {
    if (compAttr->isLiteral()) {
        return WithType::NAME;
    }

    if (compAttr->isInteger()) {
        return WithType::INTEGER;
    }

    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(compAttr);
    SynonymAttributeType attrType = syn->getAttr().value();

    auto it = SYNONYM_ATTRIBUTE_TYPE_TO_WITH_TYPE_MAP.find(attrType);
    if (it != SYNONYM_ATTRIBUTE_TYPE_TO_WITH_TYPE_MAP.end()) {
        return it->second;
    }

    throw QPSSemanticError();
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

    WithType lType = determineWithType(lhs);
    WithType rType = determineWithType(rhs);

    return lType == rType;
}
