#include "IfPattern.h"

IfPattern::IfPattern(ClauseArgument* ifSyn, std::vector<ClauseArgument*> args)
    : ifSyn(*ifSyn), firstArg(*args[0]), secondArg(*args[1]), thirdArg(*args[2]) {}

ClauseResult IfPattern::evaluate(PKBFacadeReader& reader) {
    return {{}, {}};
}
