#pragma once

#include <PKB/PKBClient/PKBFacadeReader.h>

#include "ClauseResult.h"

enum class ClauseType { SELECT, SUCH_THAT, PATTERN };

class QueryClause {
public:
    virtual ~QueryClause(){};
    virtual bool equals(const QueryClause& other) const = 0;
    virtual ClauseResult evaluate(PKBFacadeReader&) = 0;
    virtual bool isBooleanResult() const = 0;
    virtual bool containsSynonym(const Synonym&) const = 0;
    virtual std::vector<Synonym> getSynonyms() const = 0;
    virtual bool validateArguments(SynonymStore* store) = 0;
};
