#pragma once

#include "ParsingStrategy.h"
#include <string>

class SuchThatStrategy : public ParsingStrategy {
public:
    QueryClause execute(const std::string input) const override;
};