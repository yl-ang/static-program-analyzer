#include "ParserUtils.h"

std::string replaceAllExtraWhitespaces(const std::string& str) {
    std::string replacedString;

    std::regex whitespacePattern = std::regex("[" + QPSConstants::WHITESPACES + "]");
    std::regex consecutiveSpacePattern = std::regex(QPSConstants::SPACE + "+");

    replacedString = std::regex_replace(str, whitespacePattern, QPSConstants::SPACE);
    replacedString = std::regex_replace(replacedString, consecutiveSpacePattern, QPSConstants::SPACE);

    return trim(replacedString);
}

std::string removeAllWhitespaces(const std::string& str) {
    std::string replacedString;

    std::regex whitespacePattern = std::regex("[" + QPSConstants::WHITESPACES + "]");
    replacedString = std::regex_replace(str, whitespacePattern, "");

    return replacedString;
}

std::string removeAllQuotations(const std::string& str) {
    std::string replacedString;

    std::regex quotationsPattern = std::regex("[" + QPSConstants::QUOTATIONS + "]");
    replacedString = std::regex_replace(str, quotationsPattern, "");

    return replacedString;
}

std::string cleanQuotedIdent(const std::string& str) {
    return removeAllQuotations(removeAllWhitespaces(str));
}

std::vector<std::string> splitByDelimiter(const std::string& str, const std::string& delimiter) {
    size_t nextDelimiterIndex = str.find(delimiter);
    size_t offset = 0;

    std::string substring;
    size_t substringSize;

    std::vector<std::string> splitList;
    while (nextDelimiterIndex != std::string::npos) {
        substringSize = nextDelimiterIndex - offset;
        substring = str.substr(offset, substringSize);
        splitList.push_back(trim(substring));

        offset = nextDelimiterIndex + delimiter.size();
        nextDelimiterIndex = str.find(delimiter, offset);
    }

    if (offset <= str.length()) {
        substringSize = str.length() - offset;
        substring = str.substr(offset, substringSize);
        splitList.push_back(trim(substring));
    }

    return splitList;
}

std::tuple<std::string, std::string> substringUntilDelimiter(const std::string& str, const std::string& delimiter) {
    size_t nextDelimiterIndex = str.find(delimiter);

    if (nextDelimiterIndex == std::string::npos) {
        return std::make_tuple(str, "");
    } else {
        std::string substring = str.substr(0, nextDelimiterIndex);
        std::string remainingString = str.substr(nextDelimiterIndex + delimiter.size(), std::string::npos);
        return std::make_tuple(substring, remainingString);
    }
}

std::vector<std::string> stringToWordList(const std::string& string) {
    std::vector<std::string> wordList;
    std::stringstream ss(string);
    std::string word;
    while (ss >> word) {
        wordList.push_back(word);
    }
    return wordList;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(QPSConstants::WHITESPACES);
    std::string trimmedString;
    if (start != std::string::npos) {
        trimmedString = str.substr(start);
    }

    size_t end = trimmedString.find_last_not_of(QPSConstants::WHITESPACES);
    if (end != std::string::npos) {
        trimmedString = trimmedString.substr(0, end + 1);
    }

    return trimmedString;
}

std::vector<std::string> extractReturnResults(const std::string& str) {
    bool startsAndEndsWithAngularBracket = std::regex_match(str, std::regex("^<.*>$"));
    if (startsAndEndsWithAngularBracket) {
        std::string removedBracketsString = str.substr(1, str.size() - 2);
        return splitByDelimiter(removedBracketsString, ",");
    }
    return {str};
}

std::tuple<std::string, std::string> splitResultAndClause(const std::string& str) {
    bool hasAngularBrackets = std::regex_search(str, std::regex("<.*>"));
    if (hasAngularBrackets) {
        size_t closingBracketIndex = str.find_last_of(">");
        std::string returnResult = trim(str.substr(0, closingBracketIndex + 1));
        std::string remainingClauses = trim(str.substr(closingBracketIndex + 1));
        return std::make_tuple(returnResult, remainingClauses);
    }
    return substringUntilDelimiter(str, " ");
}

// str = "such that Follows(1, 2) pattern a("_", "_")"
// return {0, 24}
std::vector<std::string> getAllClauses(const std::string& str) {
    std::vector<std::regex> clausePatterns = {QPSRegexes::SUCHTHAT_CLAUSE,
                                            QPSRegexes::PATTERN_CLAUSE,
                                            QPSRegexes::AND_CLAUSE};

    std::vector<size_t> allClausesIndices = {};
    for (const auto& pattern : clausePatterns) {
        std::vector<size_t> clauseIndices = getClauseIndices(str, pattern);
        allClausesIndices.insert(std::end(allClausesIndices), std::begin(clauseIndices), std::end(clauseIndices));
    }

    if (allClausesIndices.size() == 0) {
        return {};
    }

    std::sort(allClausesIndices.begin(), allClausesIndices.end());

    std::vector<std::string> clauses = {};
    std::string clause;
    for (int i = 1; i < allClausesIndices.size(); i++) {
        clause = str.substr(allClausesIndices.at(i - 1), allClausesIndices.at(i) - allClausesIndices.at(i - 1));
        clauses.push_back(trim(clause));
    }
    clause = str.substr(allClausesIndices.at(allClausesIndices.size() - 1), std::string::npos);
    clauses.push_back(trim(clause));

    return clauses;
}

std::vector<size_t> getClauseIndices(const std::string& str, const std::string& clause) {
    size_t offset = 0;
    size_t clauseIndex = str.find(clause, offset);

    // no such clause found
    if (clauseIndex == std::string::npos) {
        return {};
    }

    std::vector<size_t> clausePositionList = {};
    while (clauseIndex != std::string::npos) {
        clausePositionList.push_back(clauseIndex);

        offset = clauseIndex + 1;
        clauseIndex = str.find(clause, offset);
    }

    return clausePositionList;
}

std::vector<size_t> getClauseIndices(const std::string& str, const std::regex& regexPattern) {
    std::vector<size_t> indices;
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), regexPattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        indices.push_back(match.position(0));
    }

    return indices;
}