#include <iostream>
#include <vector>
#include <string>

class QueryClause {
public:
    virtual ~QueryClause() {}
    // You can add common functionality or data members here
};

class SelectClause : public QueryClause {
public:
    SelectClause(const std::string& variable) : variable(variable) {}
    virtual std::string getType() const {
        return "SELECT";
    }
    std::string getVariable() const {
        return variable;
    }
private:
    std::string variable;
};
