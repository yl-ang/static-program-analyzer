#pragma once

#include <string>
#include <vector>

const std::string WHITESPACES = " \n\r\t\b\f";

std::vector<std::string> splitByDelimiter(const std::string string, const std::string delimiter);
std::string trim(const std::string string);
