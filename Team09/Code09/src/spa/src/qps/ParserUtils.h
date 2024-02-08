#pragma once

#include <string>
#include <vector>

const std::string WHITESPACES = " \n\r\t\b\f";

std::vector<std::string> splitByDelimiter(std::string& string, std::string& delimiter);
std::string trim(std::string& string);
