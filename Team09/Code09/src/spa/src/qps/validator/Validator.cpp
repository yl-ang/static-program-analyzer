#include "Validator.h"

void Validator::validate(std::vector<std::string>) {}

bool Validator::isValidSelectStatement(std::string) {
    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) {
        return false;
    }

    if (statement.back() != ';') {
        std::string firstWord = statement.substr(0, firstSpaceIndex + 1);
        std::string remainingStatement = statement.substr(firstSpaceIndex + 1, std::string::npos);
        bool containsSuchThat = containsSuchThatClause(remainingStatement);
        bool containsPattern = containsPatternClause(remainingStatement);
        if (!isSynonym(remainingStatement))
            return false;
        return true;
    }
    return false;
}

bool Validator::isValidDeclarationStatement(std::string) {
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

bool Validator::isValidSuchThatClause(std::string) {
    bool startsWithSuchThat = std::regex_search(suchThatClause, std::regex("such that"));
    if (!startsWithSuchThat) {
        return false;
    }

    std::string relRef = trim(suchThatClause.substr(9));  // remove such that from the clause

    return true;
}

bool Validator::isValidPatternClause(std::string) {
    bool startsWithPattern = std::regex_search(patternClause, std::regex("pattern"));
    if (!startsWithPattern) {
        return false;
    }

    std::string relRef = trim(patternClause.substr(8));

    return true;
}
