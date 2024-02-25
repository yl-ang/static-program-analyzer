#pragma once

#include <string>

#include "ClauseArgument.h"

class Literal : public ClauseArgument {
private:
    std::string value;

public:
    Literal(const std::string& v);
    std::string getValue() const override;
    bool isLiteral() const override;
    std::string getClauseType() const override;
    bool operator==(const ClauseArgument& other) const override;
};
