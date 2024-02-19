#include "Validator.h"

void Validator::validate(std::vector<std::string> statementList) {
    for (std::string statement : statementList) {
        if (isDeclarationStatement(statement)) {
            isValidDeclarationStatement(statement);
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

    std::string returnSynonym = trim(statement.substr(firstSpaceIndex, secondSpaceIndex - firstSpaceIndex));
    if (!isSynonym(returnSynonym)) {
        return false;
        throw QPSSyntaxError();
    }

    std::vector<std::string> clauseList = getAllClauses(trim(statement.substr(secondSpaceIndex, std::string::npos)));

    return true;

    return false;
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
    bool startsWithSuchThat = std::regex_search(suchThatClause, std::regex("such that"));
    if (!startsWithSuchThat) {
        return false;
    }

    std::string relRef = trim(suchThatClause.substr(9));  // remove such that from the clause

    return true;
}

bool Validator::isValidPatternClause(std::string patternClause) {
    bool startsWithPattern = std::regex_search(patternClause, std::regex("pattern"));
    if (!startsWithPattern) {
        return false;
    }

    std::string relRef = trim(patternClause.substr(8));

    return true;
}
