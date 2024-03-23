#include "PatternBuilder.h"

std::unique_ptr<Pattern> PatternBuilder::buildPattern(DesignEntityType synType, ClauseArgument* syn,
                                                      std::vector<ClauseArgument*> args) {
    switch (synType) {
    case DesignEntityType::ASSIGN:
        return std::make_unique<AssignPattern>(syn, args);
    case DesignEntityType::WHILE:
        return std::make_unique<WhilePattern>(syn, args);
    case DesignEntityType::IF:
        return std::make_unique<IfPattern>(syn, args);
    default:
        throw QPSSemanticError();
    }
}