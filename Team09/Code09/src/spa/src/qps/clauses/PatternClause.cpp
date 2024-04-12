#include "PatternClause.h"

PatternClause::PatternClause(std::shared_ptr<ClauseArgument> syn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : synonym(syn), arguments(args) {}

ClauseResult PatternClause::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    std::shared_ptr<Pattern> evaluator = getPattern();
    return evaluator->evaluate(reader, evalDb);
}

std::shared_ptr<Pattern> PatternClause::getPattern() {
    if (!pattern.has_value()) {
        std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(synonym);
        DesignEntityType synType = syn->getType();
        pattern = PatternBuilder::buildPattern(synType, syn, arguments);
    }

    return pattern.value();
}

bool PatternClause::equals(const QueryClause& other) const {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        // !( == ) is used as oppose to !=
        // This is because operator for != is not implemented
        if (!(*synonym == *(ptr->synonym))) {
            return false;
        }
        if (arguments.size() != ptr->arguments.size()) {
            return false;
        }
        for (int i = 0; i < arguments.size(); i++) {
            if (!(*arguments[i] == *(ptr->arguments[i]))) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool PatternClause::isBooleanResult() const {
    return false;
}

bool PatternClause::containsSynonym(const Synonym& syn) const {
    if (*synonym == syn) {
        return true;
    }
    for (auto arg : arguments) {
        if (*arg == syn) {
            return true;
        }
    }
    return false;
}

std::vector<Synonym> PatternClause::getSynonyms() const {
    std::vector<Synonym> synonyms;

    if (synonym->isSynonym()) {
        synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(synonym));
    }
    for (auto arg : arguments) {
        if (arg->isSynonym()) {
            synonyms.push_back(*std::dynamic_pointer_cast<Synonym>(arg));
        }
    }
    return synonyms;
}

bool PatternClause::validateArguments(SynonymStore* store) {
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(synonym);
    if (!syn->updateType(store)) {
        return false;
    }

    if (syn->getType() != DesignEntityType::ASSIGN && syn->getType() != DesignEntityType::WHILE &&
        syn->getType() != DesignEntityType::IF) {
        return false;
    }

    if (arguments[0]->isSynonym()) {
        std::shared_ptr<Synonym> fSyn = std::dynamic_pointer_cast<Synonym>((arguments[0]));
        if (!fSyn->updateType(store)) {
            return false;
        }

        if (fSyn->getType() != DesignEntityType::VARIABLE) {
            return false;
        }
    }

    std::shared_ptr<Pattern> patternChecker = getPattern();
    return patternChecker->validateArguments();
}
