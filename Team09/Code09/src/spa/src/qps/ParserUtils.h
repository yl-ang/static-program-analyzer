#pragma once

#include <string>
#include <vector>
#include <sstream>

const std::string WHITESPACES = " \n\r\t\b\f";

std::vector<std::string> splitByDelimiter(const std::string string, const std::string delimiter);
std::vector<std::string> stringToWordList(std::string string);
std::string trim(const std::string string);
