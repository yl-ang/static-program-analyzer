#pragma once

#include <string>
#include <vector>
#include "qps/clauseArguments/Synonym.h"

class SelectClauseArgument {
public:
    virtual ~SelectClauseArgument() {}  // Virtual destructor

    virtual bool isBoolean() const {
        return false;
    };

    virtual bool isSingleReturn() const {
        return false;
    };

    virtual bool isMultipleReturn() const {
        return false;
    };

    virtual std::string getSelectClauseType() const = 0;
    virtual std::vector<Synonym> getValue() const = 0;
    virtual bool operator==(const SelectClauseArgument& other) const = 0;
};
