#pragma once

#include "ParsingStrategy.h"
#include <string>

class SelectStrategy : public ParsingStrategy {
public:
    QueryClause execute(const std::string input) const override;
};