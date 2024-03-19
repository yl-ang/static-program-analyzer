#pragma once

#include <string>

#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauses/ClauseResult.h"

struct Pattern {
    virtual ClauseResult evaluate(PKBFacadeReader&) = 0;
};
