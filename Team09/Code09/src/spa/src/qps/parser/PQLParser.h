#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "qps/parser/parsingStrategies/ParsingStrategy.h"
#include "qps/parser/parsingStrategies/PatternStrategy.h"
#include "qps/parser/parsingStrategies/SuchThatStrategy.h"
#include "qps/parser/parsingStrategies/StrategyUtils.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ExpressionSpec.h"
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
    // static SuchThatClause toSTClause(std::vector<Synonym>, std::string);
    // static PatternClause toPatternClause(std::vector<Synonym>, std::string);
    // static std::vector<std::string> cleanParameters(const std::string&);
    // static std::vector<ClauseArgument*> buildSTParameters(const std::vector<Synonym>&, 
    //                                                     const std::vector<std::string>&);
    // static std::vector<ClauseArgument*> buildPatternParameters(const std::vector<Synonym>&, 
    //                                                         const std::vector<std::string>&);                                                          
    // static Synonym * buildSynonym(const std::vector<Synonym>& entities,const std::string& str);
    // static Synonym * buildExpression(const std::string& str);
public:
    static Query parse(UnparsedQuery);
    static std::vector<Synonym> parseQueryEntities(std::vector<std::string>);
    static std::vector<Synonym> findSelectClauses(std::vector<Synonym>, std::string);
    static std::vector<SuchThatClause> findSuchThatClauses(std::vector<Synonym>, std::string);
    static std::vector<PatternClause> findPatternClauses(std::vector<Synonym>, std::string);
};
