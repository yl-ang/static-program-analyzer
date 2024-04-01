#include "UnparsedClause.h"

UnparsedClause::UnparsedClause(std::string str, std::unique_ptr<ParsingStrategy>&& strategy)
    : str(str), strategy_(std::move(strategy)) {}

std::shared_ptr<QueryClause> UnparsedClause::execute() {
    std::shared_ptr<QueryClause> result = strategy_->execute(str);
    return result;
}
