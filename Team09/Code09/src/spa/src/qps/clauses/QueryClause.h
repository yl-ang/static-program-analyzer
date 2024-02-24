#pragma once

#include <PKB/PKBClient/PKBFacadeReader.h>

#include <string>

#include "../evaluator/Table.h"
#include "qps/clauseArguments/Synonym.h"

enum class ClauseType { SELECT, SUCH_THAT, PATTERN };

class QueryClause {
public:
    virtual ~QueryClause(){};
    // virtual ClauseType getType() const = 0;
    // virtual bool equals(const QueryClause& other) const = 0;
    // virtual Table evaluate(const PKBFacadeReader&) = 0;
};

class SelectClause : public QueryClause {
private:
    std::string arg;

public:
    SelectClause(const std::string& arg);
    ClauseType getType();
    bool equals(const QueryClause& other);
    std::string getArgument();
    // Table evaluate(const PKBFacadeReader&);
};
