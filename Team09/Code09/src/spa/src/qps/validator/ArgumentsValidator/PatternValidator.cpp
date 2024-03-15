#include "PatternValidator.h"

PatternValidator::PatternValidator(const std::string& syn, const std::vector<std::string>& args) {
    this->syn = syn;
    this->args = args;
}

void PatternValidator::validateSyntax() {
    if (!isSynonym(syn)) {
        throw QPSSyntaxError();
    }
    if (args.size() != 2) {
        throw QPSSyntaxError();
    }
    if (!isEntRef(args[0]) || !isExpressionSpec(args[1])) {
        throw QPSSyntaxError();
    }
}

void PatternValidator::validateSemantic(ValidatorSynonymStore* store) {
    if (!store->containsSynonym(syn, QPSConstants::ASSIGN)) {
        throw QPSSemanticError();
    }
    if (isSynonym(args[0]) && !store->containsSynonym(args[0], QPSConstants::VARIABLE)) {
        throw QPSSemanticError();
    }
}
