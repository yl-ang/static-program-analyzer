#include "Validator.h"

void Validator::validate(std::vector<std::string> statementList) {
    // statement example: {variable v; stmt s1, s2; Select v}
    // Ensure that Select is the last input
    if (statementList.empty()) {
        throw QPSSyntaxError();
    }  // DONE

    if (!isSelectStatement(statementList[statementList.size() - 1])) {
        throw QPSSyntaxError();
    }  // DONE

    bool hasSemanticError = false;
    for (std::string statement : statementList) {
        try {
            if (isDeclarationStatement(statement)) {
                validateDeclarationStatement(statement);
                synonymStore.storeSynonymWithStatement(statement);
            } else if (isSelectStatement(statement)) {
                validateSelectStatement(statement);
            } else {
                // NOTE: This is thrown when and empty string is made here
                // TODO(Han Qin): Add test case to test this
                throw QPSSyntaxError();  // DONE
            }
        } catch (QPSSemanticError e) {
            hasSemanticError = true;
        }
    }
    if (hasSemanticError) {
        throw QPSSemanticError();
    }
}

void Validator::validateDeclarationStatement(const std::string& statement) {
    if (statement.back() != ';') {
        throw QPSSyntaxError();
    }  // DONE
    std::string newStatement = statement.substr(0, statement.size() - 1);

    std::string designEntityWord;
    std::string remainingStatement;
    std::tie(designEntityWord, remainingStatement) = substringUntilDelimiter(newStatement, QPSConstants::SPACE);

    std::vector<std::string> synonymList = splitByDelimiter(trim(remainingStatement), ",");
    if (synonymList.size() == 1 && synonymList[0].empty()) {
        throw QPSSyntaxError();
    }  // NOT SURE WHAT THIS IS ACTUALLY

    for (std::string synonym : synonymList) {
        if (!isSynonym(synonym)) {
            throw QPSSyntaxError();
        }  // DONE
    }
}

void Validator::validateSelectStatement(const std::string& statement) {
    std::string selectWord;
    std::string remainingStatement;
    std::tie(selectWord, remainingStatement) = substringUntilDelimiter(statement, QPSConstants::SPACE);

    std::string resultClause;
    std::string remainingClausesStatement;
    std::tie(resultClause, remainingClausesStatement) = splitResultAndClause(trim(remainingStatement));

    validateResultClause(resultClause);

    std::vector<std::string> clauseList = getAllClauses(remainingClausesStatement);
    if (clauseList.size() == 0) {
        return;
    }

    bool hasSemanticError = false;
    for (std::string clause : clauseList) {
        try {
            if (containsSuchThatClause(clause)) {
                validateSuchThatClause(clause);
            } else if (containsPatternClause(clause)) {
                validatePatternClause(clause);
            } else if (containsAndClause(clause)) {
                // ignore for now
            } else {
                throw Exception("Not implemented: " + clause);
            }
        } catch (QPSSemanticError e) {
            hasSemanticError = true;
        }
    }

    if (hasSemanticError) {
        throw QPSSemanticError();
    }
}

void Validator::validateResultClause(const std::string& resultClause) {
    if (!isResultClause(resultClause)) {
        throw QPSSyntaxError();
    }

    // returnResults can be BOOLEAN or synonym(s)
    std::vector<std::string> retResults = extractReturnResults(resultClause);
    // Ensure that the synonym is in the variable store
    for (std::string retResult : retResults) {
        if (isSynonym(retResult) && !synonymStore.containsSynonymName(retResult)) {
            throw QPSSemanticError();
        }
    }
}

void Validator::validateSuchThatClause(const std::string& suchThatClause) {
    bool hasProperStructure =
        std::regex_match(suchThatClause, std::regex("^" + QPSConstants::SUCH_THAT + " .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        throw QPSSyntaxError();
    }

    // Remove "such that"
    std::string removedSuchThatClause = trim(suchThatClause.substr(QPSConstants::SUCH_THAT.size()));

    validateRelRef(removedSuchThatClause);
}

void Validator::validateRelRef(const std::string& relRefWord) {
    // Get string until '(' --> relRef
    std::string relRefString;
    std::string parameterString;
    std::tie(relRefString, parameterString) = substringUntilDelimiter(relRefWord, "(");

    relRefString = trim(relRefString);
    if (!isRelRef(relRefString)) {
        throw QPSSyntaxError();
    }

    // relRef Follows(*) and Parents(*) --> ('stmtRef', 'stmtRef')
    // relRef Uses and Modifies --> ('entRef', 'stmtRef')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(0, parameterString.size() - 1);  // Remove ending ')'
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");

    std::unique_ptr<ArgumentsValidator> argValidator = buildArgValidator(relRefString, refs);
    argValidator->validateSyntax();
    argValidator->validateSemantic(&synonymStore);
}

void Validator::validatePatternClause(const std::string& patternClause) {
    // Remove "pattern"
    std::string removedPatternClause = trim(patternClause.substr(QPSConstants::PATTERN.size()));

    validatePattern(removedPatternClause);
}

void Validator::validatePattern(const std::string& pattern) {
    // Get string until '(' --> syn-assign
    std::string synString;
    std::string parameterString;
    std::tie(synString, parameterString) = substringUntilDelimiter(pattern, "(");

    synString = trim(synString);

    // ('entRef', 'expression-spec')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(0, parameterString.size() - 1);  //  Remove ending ')'
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");

    std::unique_ptr<ArgumentsValidator> patternArgValidator = buildPatternValidator(synString, refs);
    patternArgValidator->validateSyntax();
    patternArgValidator->validateSemantic(&synonymStore);
}

std::unique_ptr<ArgumentsValidator> Validator::buildArgValidator(const std::string& relRefString,
                                                                 const std::vector<std::string>& arguments) {
    if (relRefString == QPSConstants::FOLLOWS || relRefString == QPSConstants::FOLLOWS_STAR) {
        return std::make_unique<FollowsValidator>(arguments);
    } else if (relRefString == QPSConstants::PARENT || relRefString == QPSConstants::PARENT_STAR) {
        return std::make_unique<ParentValidator>(arguments);
    } else if (relRefString == QPSConstants::USES) {
        return std::make_unique<UsesValidator>(arguments);
    } else if (relRefString == QPSConstants::MODIFIES) {
        return std::make_unique<ModifiesValidator>(arguments);
    } else if (relRefString == QPSConstants::NEXT || relRefString == QPSConstants::NEXT_STAR) {
        return std::make_unique<NextValidator>(arguments);
    } else {
        throw Exception("Not implemented: " + relRefString);
    }
}

std::unique_ptr<ArgumentsValidator> Validator::buildPatternValidator(const std::string& synonymString,
                                                                     const std::vector<std::string>& arguments) {
    return std::make_unique<PatternValidator>(synonymString, arguments);
}
