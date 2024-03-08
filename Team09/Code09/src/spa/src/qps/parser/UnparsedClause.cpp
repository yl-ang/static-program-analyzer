#include "UnparsedClause.h"

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

UnparsedClause::UnparsedClause(const std::string input, std::unique_ptr<ParsingStrategy> &&strategy)
    : input(input), strategy_(std::move(strategy)) {}
    
void UnparsedClause::set_strategy(std::unique_ptr<ParsingStrategy> &&strategy){
    strategy_ = std::move(strategy);
}
    
void UnparsedClause::execute() {
    
}