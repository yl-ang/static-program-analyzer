#pragma once

#include "ParsingStrategy.h"
#include "qps/clauses/WithClause.h"
#include "qps/validator/Validator.h"

class WithStrategy : public ParsingStrategy {
private:
    static std::vector<std::shared_ptr<ClauseArgument>> buildWithParameters(const std::vector<std::string>&);

public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
