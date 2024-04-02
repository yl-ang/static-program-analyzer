#pragma once

#include <PKB/PKBClient/PKBFacadeReader.h>

#include <algorithm>
#include <set>

#include "ClauseResult.h"
#include "SynonymValuesRetriever.h"

enum class ClauseType { SELECT, SUCH_THAT, PATTERN };  // Not used

class QueryClause {
private:
    bool negate = false;

protected:
    virtual ClauseResult evaluate(PKBFacadeReader&) = 0;

public:
    virtual ~QueryClause() {}
    virtual bool equals(const QueryClause& other) const = 0;
    virtual bool isBooleanResult() const = 0;
    virtual bool containsSynonym(const Synonym&) const = 0;
    virtual std::vector<Synonym> getSynonyms() const = 0;
    virtual bool validateArguments(SynonymStore* store) = 0;

    void setNegation(bool n);
    std::string rowValuesToString(const RowValues& row);
    ClauseResult runEvaluation(PKBFacadeReader& pkb);
};
