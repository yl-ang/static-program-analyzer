#include "ParserUtils.h"

std::string replaceAllExtraWhitespaces(const std::string& str) {
    std::string replacedString;

    std::regex whitespacePattern = std::regex("[" + QPSConstants::WHITESPACES + "]");
    std::regex consecutiveSpacePattern = std::regex(QPSConstants::SPACE + "+");

    replacedString = std::regex_replace(str, whitespacePattern, QPSConstants::SPACE);
    replacedString = std::regex_replace(replacedString, consecutiveSpacePattern, QPSConstants::SPACE);

    return trim(replacedString);
}

std::string removeCharsFrom(const std::string& str, const std::string& toRemoveChar) {
    std::string pattern = "[" + toRemoveChar + "]";
    return std::regex_replace(str, std::regex(pattern), "");
}

std::string cleanQuotedIdent(const std::string& str) {
    return removeCharsFrom(removeCharsFrom(str, QPSConstants::SPACE), QPSConstants::QUOTATIONS);
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

// str = "such that Follows(1, 2) pattern a("_", "_")"
// return {0, 24}
std::vector<std::string> getAllClauses(const std::string& str) {
    std::vector<std::regex> clausePatterns = {std::regex(QPSConstants::SELECT_CLAUSE),
                                              std::regex(QPSConstants::SUCHTHAT_CLAUSE),
                                              std::regex(QPSConstants::PATTERN_CLAUSE),
                                              std::regex(QPSConstants::WITH_CLAUSE),
                                              std::regex(QPSConstants::AND_CLAUSE)};

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
    clause = str.substr(allClausesIndices.at(allClausesIndices.size() - 1));
    clauses.push_back(trim(clause));
    return clauses;
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
