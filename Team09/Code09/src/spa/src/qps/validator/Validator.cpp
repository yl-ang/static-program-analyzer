#include "Validator.h"

void Validator::validate(std::vector<std::string> statementList) {
    for (std::string statement : statementList) {
        if (isDeclarationStatement(statement)) {
            if (isValidDeclarationStatement(statement)) {
                std::vector<std::string> wordList = stringToWordList(statement);
                std::string variableType = wordList[0];
                std::string variableName = wordList[1];
                variableStore.storeSynonym(variableName, variableType);
            }
        } else if (isSelectStatement(statement)) {
            isValidSelectStatement(statement);
        } else {
            throw QPSSyntaxError();
        }
    }
}

bool Validator::isValidSelectStatement(std::string statement) {
    int firstSpaceIndex = statement.find_first_of(WHITESPACES);

    // No characters after "Select"
    if (firstSpaceIndex == std::string::npos) {
        return false;
        throw QPSSyntaxError();
    }
    std::string selectWord = statement.substr(0, firstSpaceIndex);

    std::string remainingStatement = trim(statement.substr(firstSpaceIndex, std::string::npos));
    bool containsSuchThat = containsSuchThatClause(remainingStatement);
    bool containsPattern = containsPatternClause(remainingStatement);

    int secondSpaceIndex = statement.find_first_of(WHITESPACES, firstSpaceIndex + 1);
    if (secondSpaceIndex == std::string::npos) {
        return false;
        throw QPSSyntaxError();
    }

    std::string synonymToReturn = trim(statement.substr(firstSpaceIndex, secondSpaceIndex - firstSpaceIndex));
    if (!isSynonym(synonymToReturn)) {
        return false;
        throw QPSSyntaxError();
    }

    // Ensure that the synonym is in the variable store

    std::vector<std::string> clauseList = getAllClauses(trim(statement.substr(secondSpaceIndex, std::string::npos)));
    for (std::string clause : clauseList) {
        if (containsSuchThatClause(clause)) {
            isValidSuchThatClause(clause);
        } else if (containsPatternClause(clause)) {
            isValidPatternClause(clause);
        } else {
            return false;
            throw QPSSyntaxError();
        }
    }

    return true;
}

bool Validator::isValidDeclarationStatement(std::string statement) {
    if (statement.back() != ';') {
        return false;
    }
    statement.pop_back();

    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) {
        return false;
    }

    std::string firstWord = statement.substr(0, firstSpaceIndex + 1);  // design-entity
    std::string remainingStatement = statement.substr(firstSpaceIndex + 1, std::string::npos);

    std::vector<std::string> synonymList = splitByDelimiter(trim(remainingStatement), ",");
    if (synonymList.size() == 0) {
        return false;
    }

    for (std::string synonym : synonymList) {
        if (!isSynonym(synonym)) {
            return false;
        }
    }
    return true;
}

bool Validator::isValidSuchThatClause(std::string suchThatClause) {
    bool hasProperStructure = std::regex_match(suchThatClause, std::regex("^such that .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        return false;
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
        return false;
        throw QPSSyntaxError();
    }

    // relRef Follows(*) and Parents(*) --> ('stmtRef', 'stmtRef')
    // relRef Uses and Modifies --> ('entRef', 'stmtRef')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(1, parameterString.size() - 1);
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");
    if (refs.size() != 2) {
        return false;
        throw QPSSyntaxError();
    }

    // TODO(Han Qin): Semantic Error QPS Grammar other rules 4 & 5.
    bool hasCorrectRefFormat = false;
    if (relRefString == "Follows" || relRefString == "Follows*" || relRefString == "Parent" ||
        relRefString == "Parent*") {
        hasCorrectRefFormat = isStmtRef(refs[0]) && isStmtRef(refs[1]);
    } else if (relRefString == "Uses" || relRefString == "Modifies") {
        hasCorrectRefFormat = isStmtRef(refs[0]) && isEntRef(refs[1]);
    }

    // Ensure that synonym is in the variable store
    bool hasVariable = true;
    for (const std::string& ref : refs) {
        if (isSynonym(ref)) {
            hasVariable = hasVariable && variableStore.containsSynonymName(ref);
        }
    }

    if (!hasVariable) {
        return false;
        throw QPSSemanticError();
    }

    if (!hasCorrectRefFormat) {
        return false;
        throw QPSSyntaxError();
    }

    return true;
}

bool Validator::isValidPatternClause(std::string patternClause) {
    bool hasProperStructure = std::regex_match(patternClause, std::regex("^pattern .*\\([^)]*\\)$"));
    if (!hasProperStructure) {
        return false;
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
        return false;
        throw QPSSyntaxError();
    }

    // Semantic Error: QPS Grammar other rules 3
    bool isSynAssign = variableStore.containsSynonym(synString, "assign");
    if (!isSynAssign) {
        return false;
        throw QPSSemanticError();
    }

    // ('entRef', 'expression-spec')
    parameterString = trim(parameterString);
    parameterString = parameterString.substr(1, parameterString.size() - 1);
    std::vector<std::string> refs = splitByDelimiter(parameterString, ",");

    if (refs.size() != 2) {
        return false;
        throw QPSSyntaxError();
    }

    bool hasCorrectFormat = isEntRef(refs[0]) && isExpressionSpec(refs[1]);
    if (!hasCorrectFormat) {
        return false;
        throw QPSSyntaxError();
    }

    // Ensure that the synonyms are in the variable store
    // Semantic Error: QPS Grammar other rules 6
    bool isVariableSynonym = true;
    if (isSynonym(refs[0])) {
        isVariableSynonym = variableStore.containsSynonym(refs[0], "variable");
    }

    if (!isVariableSynonym) {
        return false;
        throw QPSSemanticError();
    }

    return true;
}

void VariableStore::storeSynonym(const std::string& synName, const std::string& synType) {
    auto variable = storage.find(synName);

    // Semantic Error: QPS Grammar other rules 1
    if (variable == storage.end()) {
        storage.insert({synName, synType});
    } else {
        throw QPSSemanticError();
    }
}

bool VariableStore::containsSynonym(const std::string& synName, const std::string& synType) {
    auto variable = storage.find(synName);
    if (variable == storage.end()) {
        return false;
    }

    return variable->second == synType;
}

bool VariableStore::containsSynonymName(const std::string& synName) {
    auto variable = storage.find(synName);
    return !(variable == storage.end());
}
