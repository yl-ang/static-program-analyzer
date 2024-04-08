#pragma once

#include <string>

#include "Exception.h"

const std::string STRATEGY_PATTERN_ERR_MSG_BASE = "Issues with Strategy Pattern for ";

class QPSStrategyPatternError : public Exception {
public:
    QPSStrategyPatternError(std::string errorClause);
};
