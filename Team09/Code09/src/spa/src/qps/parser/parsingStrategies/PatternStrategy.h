#pragma once

#include "../../clauses/PatternClause.h"
#include "ParsingStrategy.h"

class PatternStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildPatternParameters(const std::vector<std::string>&);

public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
