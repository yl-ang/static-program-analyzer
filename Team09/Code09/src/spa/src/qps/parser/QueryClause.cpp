#include <iostream>
#include <vector>
#include <string>

class QueryClause {
public:
    virtual ~QueryClause() {}
    virtual std::string getType() const = 0;
    // You can add common functionality or data members here
};

class SelectClause : public QueryClause {
public:
    SelectClause(const std::string& variable) : variable(variable) {}
    virtual std::string getType() const override {
        return "SELECT";
    }
    std::string getVariable() const {
        return variable;
    }
private:
    std::string variable;
};
