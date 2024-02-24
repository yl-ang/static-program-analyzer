#pragma once

#include "ClauseArgument.h"

class Wildcard : public ClauseArgument {
public:
    bool isWildcard() const override;
    std::string getValue() const override;
    std::string getClauseType() const override;
    bool operator==(const ClauseArgument& other) const override;
};
