#pragma once

#include "../../clauses/PatternClause.h"
#include "ParsingStrategy.h"
#include "qps/validator/Validator.h"

class PatternStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildPatternParameters(const std::vector<std::string>&);
    static std::vector<ClauseArgument*> buildTwoParameters(const std::vector<std::string>&);
    static std::vector<ClauseArgument*> buildThreeParameters(const std::vector<std::string>&);

public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
