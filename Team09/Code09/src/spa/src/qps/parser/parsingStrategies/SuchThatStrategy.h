#pragma once

#include "../../clauses/SuchThatClause.h"
#include "ParsingStrategy.h"
#include "qps/validator/Validator.h"

class SuchThatStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildSTParameters(const std::vector<std::string>&);

public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
