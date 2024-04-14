#pragma once

#include <algorithm>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "QPSConstants.h"
#include "qps/exceptions/QPSSyntaxError.h"

std::string replaceAllExtraWhitespaces(const std::string& str);
std::string removeCharsFrom(const std::string&, const std::string&);
std::string cleanQuotedIdent(const std::string& str);
std::vector<std::string> splitByDelimiter(const std::string& str, const std::string& delimiter);
std::tuple<std::string, std::string> substringUntilDelimiter(const std::string& str, const std::string& delimiter);
std::string trim(const std::string& str);

std::vector<std::string> getAllClauses(const std::string& str);
std::vector<size_t> getClauseIndices(const std::string& str, const std::regex& pattern);
