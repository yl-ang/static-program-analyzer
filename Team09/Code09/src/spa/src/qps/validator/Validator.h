#pragma once

#include <string>
#include <vector>

#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSConstants.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/exceptions/QPSSyntaxError.h"
#include "qps/parser/SynonymStore.h"

class Validator {
public:
    static void validatePatternSyntax(const std::string& synonym, const std::string& arguments);
    static void validateSuchThatSyntax(const std::string& relType, const std::string& arguments);
    static void validateClauses(SynonymStore* store, std::vector<Synonym>& selectEntities,
                                std::vector<SuchThatClause>& suchThatClauses,
                                std::vector<PatternClause>& patternClauses);

private:
    static void validatePatternTwoArg(const std::vector<std::string>& arguments);
    static void validatePatternThreeArg(const std::vector<std::string>& arguments);
    static void validateStmtStmt(const std::vector<std::string>& arguments);
    static void validateStmtOREntEnt(const std::vector<std::string>& arguments);
    static void validateEntEnt(const std::vector<std::string>& arguments);
};
