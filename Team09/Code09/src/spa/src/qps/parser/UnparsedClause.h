#pragma once

#include "parsingStrategies/ParsingStrategy.h"
#include "parsingStrategies/SuchThatStrategy.h"
#include "parsingStrategies/PatternStrategy.h"
#include "parsingStrategies/StrategyUtils.h"
#include "../clauses/QueryClause.h"

class UnparsedClause {
private:
    std::vector<Synonym> entities;
    const std::string str;
    std::unique_ptr<ParsingStrategy> strategy_;
public:
    UnparsedClause(std::vector<Synonym> entities, std::string str, std::unique_ptr<ParsingStrategy> &&strategy);
    void set_strategy(std::unique_ptr<ParsingStrategy> &&strategy);
    std::unique_ptr<QueryClause> execute();
};
