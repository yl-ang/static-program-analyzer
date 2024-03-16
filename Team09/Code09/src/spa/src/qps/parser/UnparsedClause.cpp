#include "UnparsedClause.h"

UnparsedClause::UnparsedClause(std::string str, std::unique_ptr<ParsingStrategy>&& strategy)
    : str(str), strategy_(std::move(strategy)) {}

std::unique_ptr<QueryClause> UnparsedClause::execute() {
    std::unique_ptr<QueryClause> result = strategy_->execute(str);
    return result;
}
