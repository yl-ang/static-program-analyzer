#include "catch.hpp"
#include "qps/evaluator/Query.h"

// ai-gen start(copilot, 0, e)
// prompt: test that pkb works as expected
TEST_CASE("Able to create pkb") {
    PKBFacadeReader pkb({ "v1", "v2" }, { "c1", "c2" }, { "p1", "p2" });

    REQUIRE(pkb.getVariables() == std::unordered_set<std::string>({ "v1", "v2" }));
    REQUIRE(pkb.getConstants() == std::unordered_set<std::string>({ "c1", "c2" }));
    REQUIRE(pkb.getProcedures() == std::unordered_set<std::string>({ "p1", "p2" }));
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: test that Query's evaluate works as expected
TEST_CASE("Evaluate query with select statement with only 1 synonym and no other clauses") {
    PKBFacadeReader pkb({ "v1", "v2" }, { "c1", "c2" }, { "p1", "p2" });

    QueryEntity selectEntity{ EntityType::VARIABLE, "v" };
    Query q{ selectEntity };
    std::vector<std::string> result = q.evaluate(pkb);

    REQUIRE(result == std::vector<std::string>({ "v1", "v2" }));
}
// ai-gen end
