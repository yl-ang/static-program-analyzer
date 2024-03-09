#pragma once

#include "ParsingStrategy.h"
#include <string>

class SelectStrategy : public ParsingStrategy {
public:
    std::unique_ptr<QueryClause> execute(std::vector<Synonym> entities, std::string str) const override;
};