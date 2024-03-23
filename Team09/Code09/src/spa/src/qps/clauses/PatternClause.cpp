#include "PatternClause.h"

PatternClause::PatternClause(ClauseArgument* syn, std::vector<ClauseArgument*> args) : synonym(*syn), args(args) {}

ClauseResult PatternClause::evaluate(PKBFacadeReader& reader) {
    Synonym& syn = dynamic_cast<Synonym&>(synonym);
    DesignEntityType synType = syn.getType();
    std::unique_ptr<Pattern> evaluator = PatternBuilder::buildPattern(synType, &syn, args);
    return evaluator->evaluate(reader);
}

bool PatternClause::equals(const QueryClause& other) const {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        // !( == ) is used as oppose to !=
        // This is because operator for != is not implemented
        if (!(synonym == ptr->synonym)) {
            return false;
        }
        if (args.size() != ptr->args.size()) {
            return false;
        }
        for (int i = 0; i < args.size(); i++) {
            if (!(*args[i] == *(ptr->args[i]))) {
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
    if (synonym == syn) {
        return true;
    }
    for (auto arg : args) {
        if (*arg == syn) {
            return true;
        }
    }
    return false;
}

std::vector<Synonym> PatternClause::getSynonyms() const {
    std::vector<Synonym> synonyms;

    if (synonym.isSynonym()) {
        synonyms.push_back(dynamic_cast<Synonym&>(synonym));
    }
    for (auto arg : args) {
        if ((*arg).isSynonym()) {
            synonyms.push_back(dynamic_cast<Synonym&>(*arg));
        }
    }
    return synonyms;
}

bool PatternClause::validateArguments(SynonymStore* store) {
    Synonym& syn = dynamic_cast<Synonym&>(synonym);
    if (!syn.updateType(store)) {
        return false;
    }

    if ((*args[0]).isSynonym()) {
        Synonym* fSyn = dynamic_cast<Synonym*>((args[0]));
        if (!fSyn->updateType(store)) {
            return false;
        }

        if (fSyn->getType() != DesignEntityType::VARIABLE) {
            return false;
        }
    }

    DesignEntityType synType = syn.getType();
    switch (synType) {
    case DesignEntityType::WHILE:
        if (!(*args[1]).isWildcard()) {
            throw QPSSyntaxError();
        }
        [[fallthrough]];
    case DesignEntityType::ASSIGN:
        if (args.size() != 2) {
            throw QPSSyntaxError();
        }
        break;
    case DesignEntityType::IF:
        if (args.size() != 3) {
            throw QPSSyntaxError();
        }
        break;
    default:
        return false;
    }
    return true;
}
