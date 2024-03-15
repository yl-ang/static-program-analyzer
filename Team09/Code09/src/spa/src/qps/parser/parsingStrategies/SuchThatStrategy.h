#pragma once

#include "../../clauses/SuchThatClause.h"
#include "ParsingStrategy.h"

class SuchThatStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildSTParameters(const std::vector<std::string>&);

public:
    std::unique_ptr<QueryClause> execute(std::string str) const override;
};
