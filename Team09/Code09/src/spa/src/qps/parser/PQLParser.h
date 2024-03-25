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
#include "qps/parser/SelectEntContainer.h"
#include "qps/parser/SynonymStore.h"
#include "qps/parser/parsingStrategies/ParsingStrategy.h"
#include "qps/parser/parsingStrategies/PatternStrategy.h"
#include "qps/parser/parsingStrategies/SuchThatStrategy.h"

typedef std::vector<std::string> UnparsedQueries;

class PQLParser {
protected:
    static std::shared_ptr<SelectEntContainer> parseSelectClause(std::string);
    static SuchThatClause parseSuchThatClauses(std::string);
    static PatternClause parsePatternClauses(std::string);
    static std::string getQueryClauses(UnparsedQueries);
    static SynonymStore parseQueryEntities(std::vector<std::string>);
    static void modifyClauseList(std::vector<std::string>&);
    static std::tuple<std::shared_ptr<SelectEntContainer>, std::vector<SuchThatClause>, std::vector<PatternClause>>
    parseClauses(const std::vector<std::string>&);

public:
    static Query parse(UnparsedQueries);
};
