#include <string>
#include <vector>

#include "UnparsedClause.h"
#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSRegexes.h"
#include "qps/clauses/QueryClause.h"
#include "qps/evaluator/Query.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"
#include "qps/parser/SynonymStore.h"
#include "qps/parser/parsingStrategies/ParsingStrategy.h"
#include "qps/parser/parsingStrategies/PatternStrategy.h"
#include "qps/parser/parsingStrategies/StrategyUtils.h"
#include "qps/parser/parsingStrategies/SuchThatStrategy.h"

typedef std::vector<std::string> UnparsedQueries;

class PQLParser {
private:
    static void validateClauses(SynonymStore* store, std::vector<Synonym>& selectEntities,
                                std::vector<SuchThatClause>& suchThatClauses,
                                std::vector<PatternClause>& patternClauses);
    static std::string getQueryClauses(UnparsedQueries);
    static std::vector<std::string> searchClause(const std::regex&, const std::string&);
    
public:
    static void modifyClauseList(std::vector<std::string>&);
    static Query parse(UnparsedQueries);
    static SynonymStore parseQueryEntities(std::vector<std::string>);
    static std::vector<Synonym> findSelectClauses(std::string);
    static std::vector<SuchThatClause> parseSuchThatClauses(std::vector<std::string>);
    static std::vector<PatternClause> parsePatternClauses(std::vector<std::string>);
};
