#include "WhilePattern.h"

WhilePattern::WhilePattern(ClauseArgument* whileSyn, std::vector<ClauseArgument*> args)
    : whileSyn(*whileSyn), firstArg(*args[0]), secondArg(*args[1]) {}

ClauseResult WhilePattern::evaluate(PKBFacadeReader& reader) {
    return {{}, {}};
}
