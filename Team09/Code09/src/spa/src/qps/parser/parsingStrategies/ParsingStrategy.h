#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

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

class ParsingStrategy {
public:
    virtual ~ParsingStrategy(){};
    virtual QueryClause execute(const std::string input) const = 0;
};