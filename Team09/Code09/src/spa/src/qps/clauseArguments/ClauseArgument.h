#pragma once

#include <string>

class ClauseArgument {
public:
    virtual ~ClauseArgument() {} // Virtual destructor

    virtual bool isExpressionSpec() const {
        return false;
    };

    virtual bool isInteger() const {
        return false;
    };

    virtual bool isLiteral() const {
        return false;
    };

    virtual bool isSynonym() const {
        return false;
    };

    virtual bool isWildcard() const {
        return false;
    };

    virtual std::string getClauseType() const = 0;
    virtual std::string getValue() const = 0;
    virtual bool operator==(const ClauseArgument& other) const = 0;
};
