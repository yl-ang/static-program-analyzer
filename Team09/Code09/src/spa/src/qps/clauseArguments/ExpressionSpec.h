#pragma once

#include <string>

#include "ClauseArgument.h"

class ExpressionSpec : public ClauseArgument {
private:
    std::string value;

public:
    ExpressionSpec(const std::string& v);
    bool operator==(const ClauseArgument& other) const override;

    std::string getValue() const override;
    bool isExpressionSpec() const override;
};
