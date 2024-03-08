#pragma once

#include <regex>
#include <string>

#include "ParserUtils.h"
#include "QPSConstants.h"

bool isIdent(const std::string&);
bool isName(const std::string&);
bool isInteger(const std::string&);
bool isSynonym(const std::string&);
bool isWildcard(const std::string&);
bool isQuotedIdent(const std::string&);

bool isStmtRef(const std::string&);
bool isEntRef(const std::string&);
bool isRelRef(const std::string&);

bool isExpressionSpec(const std::string&);
bool isQuotedExpr(const std::string& str);
bool isExpr(const std::string&);
bool isTerm(const std::string&);
bool isFactor(const std::string&);

bool isSelectStatement(const std::string&);
bool isDeclarationStatement(const std::string&);

bool containsSuchThatClause(const std::string&);
bool containsPatternClause(const std::string&);
