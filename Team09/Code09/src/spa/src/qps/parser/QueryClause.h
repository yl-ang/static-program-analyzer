#ifndef QUERYCLAUSE_H
#define QUERYCLAUSE_H

#include <string>
#include "QueryEntity.h"

class QueryClause {
public:
    virtual ~QueryClause() {}
};

class SelectClause : public QueryClause {
public:
    SelectClause(const std::string &arg);
    virtual std::string getType() const;
    std::string getArgument() const;
private:
    std::string arg;
};

#endif
