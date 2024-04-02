#include "PatternClause.h"

PatternClause::PatternClause(std::shared_ptr<ClauseArgument> syn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : synonym(syn), arguments(args) {}

ClauseResult PatternClause::evaluate(PKBFacadeReader& reader) {
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(synonym);
    DesignEntityType synType = syn->getType();
    std::unique_ptr<Pattern> evaluator = PatternBuilder::buildPattern(synType, syn, arguments);
    return evaluator->evaluate(reader);
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

    if ((*arguments[0]).isSynonym()) {
        std::shared_ptr<Synonym> fSyn = std::dynamic_pointer_cast<Synonym>((arguments[0]));
        if (!fSyn->updateType(store)) {
            return false;
        }

        if (fSyn->getType() != DesignEntityType::VARIABLE) {
            return false;
        }
    }

    DesignEntityType synType = syn->getType();
    switch (synType) {
    case DesignEntityType::WHILE:
        if (!(*arguments[1]).isWildcard()) {
            throw QPSSyntaxError();
        }
        [[fallthrough]];
    case DesignEntityType::ASSIGN:
        if (arguments.size() != 2) {
            throw QPSSyntaxError();
        }
        break;
    case DesignEntityType::IF:
        if (arguments.size() != 3) {
            throw QPSSyntaxError();
        }
        break;
    default:
        return false;
    }
    return true;
}
