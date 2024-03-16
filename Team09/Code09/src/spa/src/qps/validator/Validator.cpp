#include "Validator.h"

void Validator::validatePatternSyntax(const std::string& synonym, const std::string& arguments) {
    if (!isSynonym(synonym)) {
        throw QPSSyntaxError();
    }
    std::vector<std::string> argVector = splitByDelimiter(arguments, ",");
    if (argVector.size() == 2) {
        validatePatternTwoArg(argVector);
    } else if (argVector.size() == 3) {
        validatePatternThreeArg(argVector);
    } else {
        throw QPSSyntaxError();
    }
}

void Validator::validateSuchThatSyntax(const std::string& relType, const std::string& arguments) {
    std::vector<std::string> argVector = splitByDelimiter(arguments, ",");
    if (argVector.size() != 2) {
        throw QPSSyntaxError();
    }

    if (relType == QPSConstants::FOLLOWS || relType == QPSConstants::FOLLOWS_STAR || relType == QPSConstants::PARENT ||
        relType == QPSConstants::PARENT_STAR || relType == QPSConstants::NEXT || relType == QPSConstants::NEXT_STAR ||
        relType == QPSConstants::AFFECTS) {
        validateStmtStmt(argVector);
    } else if (relType == QPSConstants::MODIFIES || relType == QPSConstants::USES) {
        validateStmtOREntEnt(argVector);
    } else if (relType == QPSConstants::CALL || relType == QPSConstants::CALLS_STAR) {
        validateEntEnt(argVector);
    } else {
        throw QPSSyntaxError();
    }
}

void Validator::validateClauses(SynonymStore* store, std::vector<Synonym>& selectEntities,
                                std::vector<SuchThatClause>& suchThatClauses,
                                std::vector<PatternClause>& patternClauses) {
    bool hasSemanticError = false;
    if (!store->isValidStore()) {
        hasSemanticError = true;
    }

    if (selectEntities.size() == 1 && isBoolean(selectEntities[0].getValue())) {
        if (!selectEntities[0].updateType(&entities)) {
            selectEntities.erase(selectEntities.begin());
        }
        // Update 'BOOLEAN' if it is Synonym, remove if cannot be found
        // Evaluator will know if BOOLEAN if vector is empty
        // Do not trigger a warning should it not be a Synonym type
    } else {
        for (Synonym& syn : selectEntities) {
            if (!syn.updateType(store)) {
                hasSemanticError = true;
            }
        }
    }

    for (SuchThatClause& clause : suchThatClauses) {
        if (!clause.validateArguments(store)) {
            hasSemanticError = true;
        }
    }
    for (PatternClause& clause : patternClauses) {
        if (!clause.validateArguments(store)) {
            hasSemanticError = true;
        }
    }
    if (hasSemanticError) {
        throw QPSSemanticError();
    }
}

void Validator::validatePatternTwoArg(const std::vector<std::string>& arguments) {
    if (!isEntRef(arguments[0]) || !isExpressionSpec(arguments[1])) {
        throw QPSSyntaxError();
    }
}

void Validator::validatePatternThreeArg(const std::vector<std::string>& arguments) {
    if (!isEntRef(arguments[0]) || !isWildcard(arguments[1]) || !isWildcard(arguments[2])) {
        throw QPSSyntaxError();
    }
}

void Validator::validateStmtStmt(const std::vector<std::string>& arguments) {
    if (!isStmtRef(arguments[0]) || !isStmtRef(arguments[1])) {
        throw QPSSyntaxError();
    }
}

void Validator::validateStmtOREntEnt(const std::vector<std::string>& arguments) {
    if ((!isStmtRef(arguments[0]) && !isEntRef(arguments[0])) || !isEntRef(arguments[1])) {
        throw QPSSyntaxError();
    }
}

void Validator::validateEntEnt(const std::vector<std::string>& arguments) {
    if (!isEntRef(arguments[0]) || !isEntRef(arguments[1])) {
        throw QPSSyntaxError();
    }
}
