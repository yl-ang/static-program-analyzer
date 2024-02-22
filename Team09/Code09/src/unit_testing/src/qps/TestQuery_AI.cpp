#include <algorithm>

#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/evaluator/Query.h"

// ai-gen start(copilot, 2 e)
// prompt: test that Query's evaluate works as expected
TEST_CASE(
    "Evaluate query with select statement with only 1 synonym and no other "
    "clauses") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    pfw.setVariables({"v1", "v2"});
    PKBFacadeReader pkbReader{pkb};

    std::vector<QueryEntity> selectEntities{{EntityType::VARIABLE, "v"}};
    Query q{selectEntities, {}, {}};
    std::vector<std::string> result = q.evaluate(pkbReader);
    std::sort(result.begin(), result.end());

    std::vector<std::string> expected{"v1", "v2"};
    std::sort(expected.begin(), expected.end());

    REQUIRE(result == expected);
}
// ai-gen end
