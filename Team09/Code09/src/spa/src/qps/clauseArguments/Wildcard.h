#pragma once

#include "ClauseArgument.h"

class Wildcard : public ClauseArgument {
public:
    bool isWildcard() const override;
    std::string getValue() const override;
};
