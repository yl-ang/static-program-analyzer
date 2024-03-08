#include "UnparsedClause.h"

UnparsedClause::UnparsedClause(std::vector<Synonym> entities, std::string str, std::unique_ptr<ParsingStrategy> &&strategy)
    : entities(entities), str(str), strategy_(std::move(strategy)) {}
    
void UnparsedClause::set_strategy(std::unique_ptr<ParsingStrategy> &&strategy){
    strategy_ = std::move(strategy);
}

std::unique_ptr<QueryClause> UnparsedClause::execute() {
    std::unique_ptr<QueryClause> result = strategy_->execute(entities, str);
    return result;
}