#pragma once

#include <string>
#include <vector>
#include <sstream>

const std::string WHITESPACES = " \n\r\t\b\f";
const std::string SEMICOLON = ";";

std::vector<std::string> splitByDelimiter(const std::string string, const std::string delimiter);
std::vector<std::string> stringToWordList(std::string string);
std::string trim(const std::string string);
std::string trimSemicolon(const std::string string);
