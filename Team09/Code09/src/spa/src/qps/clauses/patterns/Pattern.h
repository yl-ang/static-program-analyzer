#pragma once

#include <string>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "qps/clauses/ClauseResult.h"

struct Pattern {
    Pattern() = default;
    virtual ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) = 0;
    virtual bool validateArguments() = 0;
};
