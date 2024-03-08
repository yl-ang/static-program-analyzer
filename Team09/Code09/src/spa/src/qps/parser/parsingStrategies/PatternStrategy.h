#pragma once

#include "ParsingStrategy.h"

#include "../../clauses/PatternClause.h"

class PatternStrategy : public ParsingStrategy {
private:
    static std::vector<ClauseArgument*> buildPatternParameters(const std::vector<Synonym>&, 
                                                            const std::vector<std::string>&);                                                          
public:
    std::unique_ptr<QueryClause> execute(std::vector<Synonym> entities, std::string str) const override;
};