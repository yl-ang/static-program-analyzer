#include "GrammarUtils.h"

bool isIdent(const std::string& str) {
    return std::regex_match(str, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isName(const std::string& str) {
    return isIdent(str);
}

bool isInteger(const std::string& str) {
    return std::regex_match(str, std::regex("^(?:0|[1-9][0-9]*)$"));
}

bool isSynonym(const std::string& str) {
    return isIdent(str);
}

bool isWildcard(const std::string& str) {
    return std::regex_match(str, std::regex("^_$"));
}

bool isQuotedIdent(const std::string& str) {
    bool startsAndEndsWithQuotes = std::regex_match(str, std::regex("^\".*\"$"));
    std::string ident = trim(str.substr(1, str.size() - 2));
    return startsAndEndsWithQuotes && isIdent(ident);
}

bool isStmtRef(const std::string& str) {
    return isSynonym(str) || isWildcard(str) || isInteger(str);
}

bool isEntRef(const std::string& str) {
    return isSynonym(str) || isWildcard(str) || isQuotedIdent(str);
}

bool isRelRef(const std::string& str) {
    std::string pattern = "(" + QPSConstants::REL_REF + ")";
    return std::regex_search(str, std::regex(pattern));
}

// TODO(Han Qin): Redo isExpressionSpec after Milestone 1.
bool isExpressionSpec(const std::string& str) {
    return std::regex_match(str, std::regex("^_\\s*\"(?:(?:0|[1-9][0-9]*)|[a-zA-Z][a-zA-Z0-9]*)\"\\s*_$")) ||
        isWildcard(str);
}

bool isSelectStatement(const std::string& str) {
    return std::regex_search(str, std::regex("^" + QPSConstants::SELECT));
}

bool isDeclarationStatement(const std::string& str) {
    std::string pattern = "^(" + QPSConstants::DESIGN_ENTITIES + ")";
    return std::regex_search(str, std::regex(pattern));
}

bool containsSuchThatClause(const std::string& selectStatement) {
    return std::regex_search(selectStatement, std::regex(QPSConstants::SUCH_THAT));
}

bool containsPatternClause(const std::string& selectStatement) {
    return std::regex_search(selectStatement, std::regex(QPSConstants::PATTERN));
}
