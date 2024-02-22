#pragma once

#include <string>

#include "ClauseArgument.h"

class ExpressionSpec : public ClauseArgument {
private:
    std::string value;

public:
    ExpressionSpec(const std::string& v);
    std::string getValue() const;
    bool operator==(const ExpressionSpec& other) const;
};
