#pragma once

#include <PKB/PKBClient/PKBFacadeReader.h>

#include <string>

#include "../evaluator/Table.h"
#include "ClauseResult.h"
#include "qps/clauseArguments/Synonym.h"

enum class ClauseType { SELECT, SUCH_THAT, PATTERN };

class QueryClause {
public:
    virtual ~QueryClause(){};
    // virtual ClauseType getType() const = 0;
    // virtual bool equals(const QueryClause& other) const = 0;
    // virtual ClauseResult evaluate(const PKBFacadeReader&) = 0;
};
