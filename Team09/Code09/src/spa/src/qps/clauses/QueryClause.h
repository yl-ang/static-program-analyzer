#pragma once

#include <string>

#include "../parser/QueryEntity.h"

enum class ClauseType { SELECT, SUCH_THAT, PATTERN };

class QueryClause {
public:
    virtual ~QueryClause(){};
    virtual ClauseType getType() const = 0;
    virtual bool equals(const QueryClause& other) const = 0;
};

class SelectClause : public QueryClause {
private:
    std::string arg;

public:
    SelectClause(const std::string& arg);
    ClauseType getType() const override;
    bool equals(const QueryClause& other) const override;
    std::string getArgument() const;
};
