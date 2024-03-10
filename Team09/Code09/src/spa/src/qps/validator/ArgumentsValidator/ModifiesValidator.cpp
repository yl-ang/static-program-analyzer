#include "ModifiesValidator.h"

ModifiesValidator::ModifiesValidator(const std::vector<std::string>& args) {
    this->args = args;
}

void ModifiesValidator::validateSyntax() {
    if (args.size() != 2) {
        throw QPSSyntaxError();
    }
    if (!isStmtRef(args[0]) || !isEntRef(args[1])) {
        throw QPSSyntaxError();
    }
}

void ModifiesValidator::validateSemantic(SynonymStore* store) {
    std::string arg1 = args[0];
    std::string arg2 = args[1];

    if (isWildcard(arg1)) {
        throw QPSSemanticError();
    }

    if (isSynonym(arg1)) {
        if (!store->containsSynonymName(arg1) || store->containsSynonym(arg1, QPSConstants::CONSTANT) ||
            store->containsSynonym(arg1, QPSConstants::VARIABLE)) {
            throw QPSSemanticError();
        }
    }

    if (isSynonym(arg2) && !store->containsSynonym(arg2, QPSConstants::VARIABLE)) {
        throw QPSSemanticError();
    }
}
