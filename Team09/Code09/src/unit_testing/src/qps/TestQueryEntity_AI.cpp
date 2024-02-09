#include "catch.hpp"
#include "qps/evaluator/QueryEntity.h"

// ai-gen start(copilot, 1, e)
// prompt: write test cases that creates QueryEntity objects for each entity type
// and checks that the type and name are correctly set
TEST_CASE("Creates QueryEntity for type variable") {
    QueryEntity qe{ QueryEntity::createVariable("v") };
    REQUIRE(qe.getType() == EntityType::VARIABLE);
    REQUIRE(qe.getName() == "v");
}

TEST_CASE("Creates QueryEntity for type procedure") {
    QueryEntity qe{ QueryEntity::createProcedure("p") };
    REQUIRE(qe.getType() == EntityType::PROCEDURE);
    REQUIRE(qe.getName() == "p");
}

TEST_CASE("Creates QueryEntity for type constant") {
    QueryEntity qe{ QueryEntity::createConstant("c") };
    REQUIRE(qe.getType() == EntityType::CONSTANT);
    REQUIRE(qe.getName() == "c");
}
// ai-gen end
