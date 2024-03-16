#pragma once

#include <string>
#include <vector>

#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSConstants.h"
#include "qps/exceptions/QPSSyntaxError.h"

class Validator {
public:
    static void validatePatternSyntax(const std::string& synonym, const std::string& arguments);
    static void validateSuchThatSyntax(const std::string& relType, const std::string& arguments);

private:
    static void validatePatternTwoArg(const std::vector<std::string>& arguments);
    static void validatePatternThreeArg(const std::vector<std::string>& arguments);
    static void validateStmtStmt(const std::vector<std::string>& arguments);
    static void validateStmtOREntEnt(const std::vector<std::string>& arguments);
    static void validateEntEnt(const std::vector<std::string>& arguments);
};
