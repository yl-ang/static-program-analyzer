#pragma once

#include "../../clauses/PatternClause.h"
#include "ParsingStrategy.h"
#include "qps/validator/Validator.h"

class PatternStrategy : public ParsingStrategy {
private:
    static std::vector<std::shared_ptr<ClauseArgument>> buildPatternParameters(const std::vector<std::string>&);
    static std::vector<std::shared_ptr<ClauseArgument>> buildTwoParameters(const std::vector<std::string>&);
    static std::vector<std::shared_ptr<ClauseArgument>> buildThreeParameters(const std::vector<std::string>&);

public:
    std::shared_ptr<QueryClause> execute(std::string str) const override;
};
