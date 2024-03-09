#include "AssignPatternValidator.h"

AssignPatternValidator::AssignPatternValidator(const std::vector<std::string>& args) {
    this->args = args;
}

void AssignPatternValidator::validateSyntax() {
    if (args.size() != 2) {
        throw QPSSyntaxError();
    }
    if (!isEntRef(args[0]) || !isExpressionSpec(args[1])) {
        throw QPSSyntaxError();
    }
}

void AssignPatternValidator::validateSemantic(SynonymStore* store) {
    bool isVariableSynonym = true;
    if (isSynonym(args[0]) && !store->containsSynonym(args[0], QPSConstants::VARIABLE)) {
        throw QPSSemanticError();
    }
}
