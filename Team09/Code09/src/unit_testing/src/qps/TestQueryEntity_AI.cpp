#include "catch.hpp"
#include "qps/parser/QueryEntity.h"

// helper functions
static QueryEntity createVariable(const std::string& name) {
    return {DesignEntityType::VARIABLE, name};
}

static QueryEntity createProcedure(const std::string& name) {
    return {DesignEntityType::PROCEDURE, name};
}

static QueryEntity createConstant(const std::string& name) {
    return {DesignEntityType::CONSTANT, name};
}

// ai-gen start(copilot, 1, e)
// prompt: write test cases that creates QueryEntity objects for each entity type
// and checks that the type and name are correctly set

TEST_CASE("Creates QueryEntity for type variable1") {
    QueryEntity qe{createVariable("v")};
    REQUIRE(qe.getType() == DesignEntityType::VARIABLE);
    REQUIRE(qe.getName() == "v");
}

TEST_CASE("Creates QueryEntity for type procedure") {
    QueryEntity qe{createProcedure("p")};
    REQUIRE(qe.getType() == DesignEntityType::PROCEDURE);
    REQUIRE(qe.getName() == "p");
}

TEST_CASE("Creates QueryEntity for type constant") {
    QueryEntity qe{createConstant("c")};
    REQUIRE(qe.getType() == DesignEntityType::CONSTANT);
    REQUIRE(qe.getName() == "c");
}

// ai-gen end
