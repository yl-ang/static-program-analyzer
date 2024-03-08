#pragma once

#include "ParsingStrategy.h"

#include "../../clauses/SuchThatClause.h"
#include "StrategyUtils.h"

class SuchThatStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildSTParameters(const std::vector<Synonym>&, 
                                                        const std::vector<std::string>&);                                                          
public:
    std::unique_ptr<QueryClause> execute(std::vector<Synonym> entities, std::string str) const override;
};