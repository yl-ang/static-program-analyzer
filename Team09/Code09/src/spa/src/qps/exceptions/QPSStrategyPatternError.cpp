#include "QPSStrategyPatternError.h"

QPSStrategyPatternError::QPSStrategyPatternError(std::string errorClause)
    : Exception(STRATEGY_PATTERN_ERR_MSG_BASE + errorClause) {}
