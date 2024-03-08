#pragma once

#include <string>
#include <vector>

#include "../../GrammarUtils.h"
#include "../../ParserUtils.h"
#include "StrategyUtils.h"
#include "../../QPSRegexes.h"
#include "../../exceptions/Exception.h"
#include "../../clauses/QueryClause.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/ExpressionSpec.h"

class ParsingStrategy {
public:
    virtual ~ParsingStrategy(){};
    virtual std::unique_ptr<QueryClause> execute(std::vector<Synonym> entities, std::string str) const = 0;
};