#include "Validator.h"

void Validator::validate(std::vector<std::string> statementList) {
    for (std::string statement : statementList) {
        if (isDeclarationStatement(statement)) {
            validateDeclarationStatement(statement);

            std::vector<std::string> wordList = stringToWordList(statement.substr(0, statement.size() - 1));
            std::string variableType = wordList[0];
            std::string variableName = wordList[1];
            synonymStore.storeSynonym(variableName, variableType);
        } else if (isSelectStatement(statement)) {
            validateSelectStatement(statement);
        } else {
            throw QPSSyntaxError();
        }
    }
}

void Validator::validateSelectStatement(const std::string& statement) {
    int firstSpaceIndex = statement.find_first_of(WHITESPACES);

    // No characters after "Select"
    if (firstSpaceIndex == std::string::npos) {
        throw QPSSyntaxError();
    }
    std::string selectWord = statement.substr(0, firstSpaceIndex);

    std::string remainingStatement = trim(statement.substr(firstSpaceIndex, std::string::npos));
    bool containsSuchThat = containsSuchThatClause(remainingStatement);
    bool containsPattern = containsPatternClause(remainingStatement);

    int secondSpaceIndex = statement.find_first_of(WHITESPACES, firstSpaceIndex + 1);
    std::string synonymToReturn;
    std::string remainingClausesStatement = "";
    if (secondSpaceIndex == std::string::npos) {
        synonymToReturn = trim(statement.substr(firstSpaceIndex, std::string::npos));
        if (synonymToReturn == "") {
            throw QPSSyntaxError();
        }
    } else {
        synonymToReturn = trim(statement.substr(firstSpaceIndex, secondSpaceIndex - firstSpaceIndex));
        remainingClausesStatement = trim(statement.substr(secondSpaceIndex, std::string::npos));
    }

    if (!isSynonym(synonymToReturn)) {
        throw QPSSyntaxError();
    }

    // Ensure that the synonym is in the variable store
    bool hasSynonym = synonymStore.containsSynonymName(synonymToReturn);
    if (!hasSynonym) {
        throw QPSSemanticError();
    }

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
            throw QPSSyntaxError();
        }
    }
}

void Validator::validateDeclarationStatement(const std::string& statement) {
    if (statement.back() != ';') {
        throw QPSSyntaxError();
    }
    std::string newStatement = statement.substr(0, statement.size() - 1);

    int firstSpaceIndex = newStatement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) {
        throw QPSSyntaxError();
    }

    std::string firstWord = newStatement.substr(0, firstSpaceIndex + 1);  // design-entity
    std::string remainingStatement = newStatement.substr(firstSpaceIndex + 1, std::string::npos);

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

void Validator::validateSuchThatClause(const std::string& suchThatClause) {
    bool hasProperStructure = std::regex_match(suchThatClause, std::regex("^such that .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        throw QPSSyntaxError();
    }

    // Remove "such that"
    std::string removedSuchThatClause = trim(suchThatClause.substr(9));

    // Get string until '(' --> relRef
    std::string relRefString;
    std::string parameterString;
    std::tie(relRefString, parameterString) = substringUntilDelimiter(removedSuchThatClause, "(");

    // Depending on the relRef do checks
    relRefString = trim(relRefString);
    std::regex relRefPatterns = std::regex("Follows|Follows*|Parent|Parent*|Uses|Modifies");
    bool isRelRef = std::regex_search(relRefString, relRefPatterns);
    if (!isRelRef) {
        throw QPSSyntaxError();
    }

    // relRef Follows(*) and Parents(*) --> ('stmtRef', 'stmtRef')
    // relRef Uses and Modifies --> ('entRef', 'stmtRef')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(0, parameterString.size() - 1);  // Remove ending ')'
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");
    if (refs.size() != 2) {
        throw QPSSyntaxError();
    }

    std::string arg1, arg2 = refs[0], refs[1];
    bool hasCorrectRefFormat = false;
    if (relRefString == "Follows" || relRefString == "Follows*" || relRefString == "Parent" ||
        relRefString == "Parent*") {
        hasCorrectRefFormat = isStmtRef(arg1) && isStmtRef(arg2);
    } else if (relRefString == "Uses" || relRefString == "Modifies") {
        // Semantic Error QPS Grammar other rules 4
        if (isWildcard(arg1)) {
            throw QPSSemanticError();
        }
        // Semantic Error QPS Grammar other rules 5b
        if (isSynonym(arg2) && !synonymStore.containsSynonym(arg2, "variable")) {
            throw QPSSemanticError();
        }
        hasCorrectRefFormat = isStmtRef(arg1) && isEntRef(arg2);
    }
    // Semantic Error QPS Grammar other rules 5a
    if ((relRefString == "Parent" || relRefString == "Parent*") && isSynonym(arg1) &&
        (synonymStore.containsSynonym(arg1, "variable") || synonymStore.containsSynonym(arg1, "constant") ||
         synonymStore.containsSynonym(arg1, "procedure"))) {
        throw QPSSemanticError();
    }

    // Ensure that synonym is in the variable store
    bool hasVariable = true;
    for (const std::string& ref : refs) {
        if (isSynonym(ref)) {
            hasVariable = hasVariable && synonymStore.containsSynonymName(ref);
        }
    }
    if (!hasVariable) {
        throw QPSSemanticError();
    }

    if (!hasCorrectRefFormat) {
        throw QPSSyntaxError();
    }
}

void Validator::validatePatternClause(const std::string& patternClause) {
    bool hasProperStructure = std::regex_match(patternClause, std::regex("^pattern .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        throw QPSSyntaxError();
    }

    // Remove "pattern"
    std::string removedPatternClause = trim(patternClause.substr(7));

    // Get string until '(' --> syn-assign
    std::string synString;
    std::string parameterString;
    std::tie(synString, parameterString) = substringUntilDelimiter(removedPatternClause, "(");

    synString = trim(synString);
    if (!isSynonym(synString)) {
        throw QPSSyntaxError();
    }

    // Semantic Error: QPS Grammar other rules 3
    bool isSynAssign = synonymStore.containsSynonym(synString, "assign");
    if (!isSynAssign) {
        throw QPSSemanticError();
    }

    // ('entRef', 'expression-spec')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(0, parameterString.size() - 1);  //  Remove ending ')'
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");

    if (refs.size() != 2) {
        throw QPSSyntaxError();
    }

    bool hasCorrectFormat = isEntRef(refs[0]);
    hasCorrectFormat = hasCorrectFormat && isExpressionSpec(refs[1]);
    if (!hasCorrectFormat) {
        throw QPSSyntaxError();
    }

    // Ensure that the synonyms are in the variable store
    // Semantic Error: QPS Grammar other rules 6
    bool isVariableSynonym = true;
    if (isSynonym(refs[0])) {
        isVariableSynonym = synonymStore.containsSynonym(refs[0], "variable");
    }

    if (!isVariableSynonym) {
        throw QPSSemanticError();
    }
}
