#pragma once

#include <regex>
#include <sstream>
#include <string>
#include <vector>

const std::string WHITESPACES = " \n\r\t\b\f";
const std::string SPACE = " ";

std::string replaceAllWhitespaces(std::string str);
std::vector<std::string> splitByDelimiter(const std::string str,
                                          const std::string delimiter);
std::vector<std::string> stringToWordList(std::string str);
std::string trim(const std::string str);
