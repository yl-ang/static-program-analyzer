#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "parsingStrategies/ParsingStrategy.h"
#include "../ParserUtils.h"
#include "../QPSRegexes.cpp"
#include "../clauses/QueryClause.h"
#include "../evaluator/Query.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ExpressionSpec.h"

class UnparsedClause {
private:
    std::unique_ptr<ParsingStrategy> strategy_;
    const std::string input;
public:
    explicit UnparsedClause(const std::string input, std::unique_ptr<ParsingStrategy> &&strategy);
    void set_strategy(std::unique_ptr<ParsingStrategy> &&strategy);
    QueryClause execute();
};
