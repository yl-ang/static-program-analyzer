#pragma once

#include <string>

#include "ParsingStrategy.h"

class SelectStrategy : public ParsingStrategy {
public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
