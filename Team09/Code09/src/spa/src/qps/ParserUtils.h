#pragma once

#include <algorithm>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "QPSConstants.h"

const std::string WHITESPACES = " \n\r\t\b\f";
const std::string SPACE = " ";
const std::string QUOTATIONS = "\"";

std::string replaceAllExtraWhitespaces(const std::string& str);
std::string removeAllWhitespaces(std::string str);
std::string removeAllQuotations(std::string str);
std::vector<std::string> splitByDelimiter(const std::string& str, const std::string& delimiter);
std::tuple<std::string, std::string> substringUntilDelimiter(const std::string& str, const std::string& delimiter);
std::vector<std::string> stringToWordList(const std::string& string);
std::string trim(const std::string& str);

std::tuple<std::string, std::string> splitResultAndClause(const std::string& str);
std::vector<std::string> extractReturnResults(const std::string& str);
std::vector<std::string> getAllClauses(const std::string& str);
std::vector<size_t> getClauseIndices(const std::string& str, const std::string& clause);
