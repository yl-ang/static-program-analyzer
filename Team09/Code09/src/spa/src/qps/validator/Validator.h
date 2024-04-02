#pragma once

#include <string>
#include <vector>

#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSConstants.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/WithClause.h"
#include "qps/exceptions/QPSSyntaxError.h"
#include "qps/parser/SelectEntContainer.h"
#include "qps/parser/SynonymStore.h"

class Validator {
public:
    static void validatePatternSyntax(const std::string& synonym, const std::string& arguments);
    static void validateSuchThatSyntax(const std::string& relType, const std::string& arguments);
    static void validateWithSyntax(const std::string& arguments);
    static void validateClauses(SynonymStore* store, std::shared_ptr<SelectEntContainer> selectEntities,
                                std::vector<std::shared_ptr<SuchThatClause>>& suchThatClauses,
                                std::vector<std::shared_ptr<PatternClause>>& patternClauses,
                                std::vector<std::shared_ptr<WithClause>>& withClauses);

private:
    static void validatePatternTwoArg(const std::vector<std::string>& arguments);
    static void validatePatternThreeArg(const std::vector<std::string>& arguments);
    static void validateStmtStmt(const std::vector<std::string>& arguments);
    static void validateStmtOREntEnt(const std::vector<std::string>& arguments);
    static void validateEntEnt(const std::vector<std::string>& arguments);
};
