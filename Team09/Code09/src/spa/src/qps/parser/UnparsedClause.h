#pragma once

#include <string>
#include <vector>

#include "../clauses/QueryClause.h"
#include "parsingStrategies/ParsingStrategy.h"
#include "parsingStrategies/PatternStrategy.h"
#include "parsingStrategies/SuchThatStrategy.h"

class UnparsedClause {
private:
    const std::string str;
    std::unique_ptr<ParsingStrategy> strategy_;

public:
    UnparsedClause(std::string str, std::unique_ptr<ParsingStrategy>&& strategy);
    std::shared_ptr<QueryClause> execute();
};
