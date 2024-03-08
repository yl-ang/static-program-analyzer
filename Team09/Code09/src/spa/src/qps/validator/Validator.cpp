#include "Validator.h"

void Validator::validate(std::vector<std::string> statementList) {
    // statement example: variable v / stmt s1, s2 / Select v
    for (std::string statement : statementList) {
        if (isDeclarationStatement(statement)) {
            validateDeclarationStatement(statement);
            synonymStore.storeSynonymWithStatement(statement);
        } else if (isSelectStatement(statement)) {
            validateSelectStatement(statement);
        } else {
            // NOTE: This is thrown when and empty string is made here
            // TODO(Han Qin): Add test case to test this
            throw QPSSyntaxError();
        }
    }
}

void Validator::validateDeclarationStatement(const std::string& statement) {
    if (statement.back() != ';') {
        throw QPSSyntaxError();
    }
    std::string newStatement = statement.substr(0, statement.size() - 1);

    std::string designEntityWord;
    std::string remainingStatement;
    std::tie(designEntityWord, remainingStatement) = substringUntilDelimiter(newStatement, SPACE);

    std::vector<std::string> synonymList = splitByDelimiter(trim(remainingStatement), ",");
    if (synonymList.size() == 0) {
        throw QPSSyntaxError();
    }

    for (std::string synonym : synonymList) {
        if (!isSynonym(synonym)) {
            throw QPSSyntaxError();
        }
    }
}

void Validator::validateSelectStatement(const std::string& statement) {
    std::string selectWord;
    std::string remainingStatement;
    std::tie(selectWord, remainingStatement) = substringUntilDelimiter(statement, SPACE);

    std::string returnClause;
    std::string remainingClausesStatement;
    std::tie(returnClause, remainingClausesStatement) = substringUntilDelimiter(trim(remainingStatement), SPACE);

    validateReturnClause(returnClause);

    std::vector<std::string> clauseList = getAllClauses(remainingClausesStatement);
    if (clauseList.size() == 0) {
        return;
    }

    for (std::string clause : clauseList) {
        if (containsSuchThatClause(clause)) {
            validateSuchThatClause(clause);
        } else if (containsPatternClause(clause)) {
            validatePatternClause(clause);
        } else {
            throw Exception("Not implemented: " + clause);
        }
    }
}

void Validator::validateReturnClause(const std::string& returnClause) {
    if (!isSynonym(returnClause)) {
        throw QPSSyntaxError();
    }

    // Ensure that the synonym is in the variable store
    bool hasSynonym = synonymStore.containsSynonymName(returnClause);
    if (!hasSynonym) {
        throw QPSSemanticError();
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

    ArgumentsValidator* argValidator = buildArgValidator(relRefString, refs);
    argValidator->validateSyntax();
    argValidator->validateSemantic(&synonymStore);
    delete argValidator;
}

void Validator::validatePatternClause(const std::string& patternClause) {
    bool hasProperStructure =
        std::regex_match(patternClause, std::regex("^" + QPSConstants::PATTERN + " .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        throw QPSSyntaxError();
    }

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
    if (!isSynonym(synString)) {
        throw QPSSyntaxError();
    }

    // ('entRef', 'expression-spec')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(0, parameterString.size() - 1);  //  Remove ending ')'
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");

    ArgumentsValidator* patternArgValidator = buildPatternValidator(synString, refs);
    patternArgValidator->validateSyntax();
    patternArgValidator->validateSemantic(&synonymStore);
    delete patternArgValidator;
}

ArgumentsValidator* Validator::buildArgValidator(const std::string& relRefString,
                                                 const std::vector<std::string>& arguments) {
    if (relRefString == QPSConstants::FOLLOWS || relRefString == QPSConstants::FOLLOWS_STAR) {
        return new FollowsValidator(arguments);
    } else if (relRefString == QPSConstants::PARENT || relRefString == QPSConstants::PARENT_STAR) {
        return new ParentValidator(arguments);
    } else if (relRefString == QPSConstants::USES) {
        return new UsesValidator(arguments);
    } else if (relRefString == QPSConstants::MODIFIES) {
        return new ModifiesValidator(arguments);
    } else {
        throw Exception("Not implemented: " + relRefString);
    }
}

ArgumentsValidator* Validator::buildPatternValidator(const std::string& synonymString,
                                                     const std::vector<std::string>& arguments) {
    if (synonymStore.containsSynonym(synonymString, QPSConstants::ASSIGN)) {
        return new AssignPatternValidator(arguments);
    } else {
        throw QPSSemanticError();
    }
}
