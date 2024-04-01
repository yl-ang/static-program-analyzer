#include "PatternBuilder.h"

std::shared_ptr<Pattern> PatternBuilder::buildPattern(DesignEntityType synType, std::shared_ptr<ClauseArgument> syn,
                                                      std::vector<std::shared_ptr<ClauseArgument>> args) {
    switch (synType) {
    case DesignEntityType::ASSIGN:
        return std::make_shared<AssignPattern>(syn, args);
    case DesignEntityType::WHILE:
        return std::make_shared<WhilePattern>(syn, args);
    case DesignEntityType::IF:
        return std::make_shared<IfPattern>(syn, args);
    default:
        throw QPSSemanticError();
    }
}
