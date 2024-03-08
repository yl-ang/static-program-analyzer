#include <string>
#include <vector>

#include "qps/parser/parsingStrategies/ParsingStrategy.h"
#include "qps/parser/parsingStrategies/PatternStrategy.h"
#include "qps/parser/parsingStrategies/SuchThatStrategy.h"
#include "qps/parser/parsingStrategies/StrategyUtils.h"
#include "UnparsedClause.h"
#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/Exception.h"
#include "../QPSRegexes.h"
#include "../clauses/QueryClause.h"
#include "../evaluator/Query.h"

typedef std::vector<std::string> UnparsedQuery;

class PQLParser {
private:
    static std::string getQueryClauses(UnparsedQuery);
    static std::vector<std::string> searchClause(const std::regex&, const std::string&);
public:
    static Query parse(UnparsedQuery);
    static std::vector<Synonym> parseQueryEntities(std::vector<std::string>);
    static std::vector<Synonym> findSelectClauses(std::vector<Synonym>, std::string);
    static std::vector<SuchThatClause> findSuchThatClauses(std::vector<Synonym>, std::string);
    static std::vector<PatternClause> findPatternClauses(std::vector<Synonym>, std::string);
};
