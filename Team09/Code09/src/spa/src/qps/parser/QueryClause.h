#include <string>
#include "QueryEntity.h"

class QueryClause {
public:
    virtual ~QueryClause() {}
    virtual std::string getType() const = 0;
};

class SelectClause : public QueryClause {
public:
    SelectClause(const std::string &arg);
    virtual std::string getType() const override;
    std::string getArgument() const;
private:
    std::string arg;
};