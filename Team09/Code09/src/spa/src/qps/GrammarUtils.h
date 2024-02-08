#pragma once

#include <regex> 
#include <string>

bool isIdent(std::string);
bool isName(std::string);
bool isInteger(std::string);
bool isSynonym(std::string);
bool isWildcard(std::string);
bool isQuotedIdent(std::string);

bool isStmtRef(std::string);
bool isEntRef(std::string);
