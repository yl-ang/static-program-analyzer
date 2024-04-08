#include <string>
#include <vector>

#include "UnparsedClause.h"
#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSRegexes.h"
#include "qps/clauses/QueryClause.h"
#include "qps/clauses/WithClause.h"
#include "qps/evaluator/Query.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"
#include "qps/parser/ElementEntContainer.h"
#include "qps/parser/SelectEntContainer.h"
#include "qps/parser/SynonymStore.h"
#include "qps/parser/TupleEntContainer.h"
#include "qps/parser/parsingStrategies/ParsingStrategy.h"
#include "qps/parser/parsingStrategies/PatternStrategy.h"
#include "qps/parser/parsingStrategies/SuchThatStrategy.h"
#include "qps/parser/parsingStrategies/WithStrategy.h"

typedef std::vector<std::string> UnparsedQueries;

class PQLParser {
protected:
    static std::shared_ptr<SelectEntContainer> parseSelectClause(std::string);
    static std::shared_ptr<SuchThatClause> parseSuchThatClauses(std::string);
    static std::shared_ptr<PatternClause> parsePatternClauses(std::string);
    static std::shared_ptr<WithClause> parseWithClauses(std::string);
    static std::string getQueryClauses(UnparsedQueries);
    static SynonymStore parseQueryEntities(std::vector<std::string>);
    static void modifyClauseList(std::vector<std::string>&);
    static std::tuple<std::shared_ptr<SelectEntContainer>, std::vector<std::shared_ptr<SuchThatClause>>,
                      std::vector<std::shared_ptr<PatternClause>>, std::vector<std::shared_ptr<WithClause>>>
    parseClauses(const std::vector<std::string>&);

public:
    static Query parse(UnparsedQueries);
};
