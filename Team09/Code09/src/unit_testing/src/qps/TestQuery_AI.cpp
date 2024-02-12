#include "catch.hpp"
#include "qps/evaluator/Query.h"
#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"

// ai-gen start(copilot, 0, e)
// prompt: test that Query's evaluate works as expected
TEST_CASE("Evaluate query with select statement with only 1 synonym and no other clauses") {
    PKB pkb{};
    PKBFacadeWriter pfw{ pkb };
    pfw.setVariables({ "v1", "v2" });
    PKBFacadeReader pkbReader{ pkb };

    QueryEntity selectEntity{ EntityType::VARIABLE, "v" };
    Query q{ selectEntity };
    std::vector<std::string> result = q.evaluate(pkbReader);

    REQUIRE(result == std::vector<std::string>({ "v1", "v2" }));
}
// ai-gen end
