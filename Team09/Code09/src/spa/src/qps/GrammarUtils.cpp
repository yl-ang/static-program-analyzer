#include "GrammarUtils.h"

bool isIdent(std::string str) {
    return std::regex_match(str, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isName(std::string str) { return isIdent(str); }

bool isInteger(std::string str) {
    return std::regex_match(str, std::regex("^(?:0|[1-9][0-9]*)$"));
}

bool isSynonym(std::string str) { return isIdent(str); }

bool isWildcard(std::string str) {
    return std::regex_match(str, std::regex("^_$"));
}

bool isQuotedIdent(std::string str) {
    return std::regex_match(str, std::regex("^\"[a-zA-Z][a-zA-Z0-9]*\"$"));
}

bool isStmtRef(std::string str) {
    return isSynonym(str) || isWildcard(str) || isInteger(str);
}

bool isEntRef(std::string str) {
    return isSynonym(str) || isWildcard(str) || isQuotedIdent(str);
}

bool isSelectStatement(std::string str) {
    return std::regex_search(str, std::regex("^Select"));
}

bool isDeclarationStatement(std::string str) {
    std::string pattern = "^(" + DesignEntity::DESIGN_ENTITIES + ")";
    return std::regex_search(str, std::regex(pattern));
}

bool isValidSelectStatement(std::string statement) {
    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) return false;
    if (statement.back() != ';') {
        std::string firstWord =
            statement.substr(0, firstSpaceIndex + 1);  // Select
        std::string remainingStatement =
            statement.substr(firstSpaceIndex + 1, std::string::npos);
        bool containsSuchThat = containsSuchThatClause(remainingStatement);
        bool containsPattern = containsPatternClause(remainingStatement);
        if (!isSynonym(remainingStatement)) return false;
        return true;
    }
    return false;
}

bool isValidDeclarationStatement(std::string statement) {
    if (statement.back() != ';') return false;
    statement.pop_back();

    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) return false;
    std::string firstWord =
        statement.substr(0, firstSpaceIndex + 1);  // design-entity
    std::string remainingStatement =
        statement.substr(firstSpaceIndex + 1, std::string::npos);

    std::vector<std::string> synonymList =
        splitByDelimiter(trim(remainingStatement), ",");
    if (synonymList.size() == 0) return false;
    for (std::string synonym : synonymList) {
        if (!isSynonym(synonym)) {
            return false;
        }
    }
    return true;
}

bool containsSuchThatClause(std::string selectStatement) {
    return std::regex_search(selectStatement, std::regex("such that"));
}

bool containsPatternClause(std::string selectStatement) {
    return std::regex_search(selectStatement, std::regex("pattern"));
}

bool isValidSuchThatClause(std::string suchThatClause) {
    bool startsWithSuchThat =
        std::regex_search(suchThatClause, std::regex("such that"));
    if (!startsWithSuchThat) {
        return false;
    }

    std::string relRef =
        trim(suchThatClause.substr(9));  // remove such that from the clause

    return true;
}

bool isValidPatternClause(std::string patternClause) {
    bool startsWithPattern =
        std::regex_search(patternClause, std::regex("pattern"));
    if (!startsWithPattern) {
        return false;
    }

    std::string relRef = trim(patternClause.substr(8));

    return true;
}
