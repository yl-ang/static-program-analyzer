#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"

// helper functions
static Synonym createVariable(const std::string& name) {
    return {DesignEntityType::VARIABLE, name};
}

static Synonym createProcedure(const std::string& name) {
    return {DesignEntityType::PROCEDURE, name};
}

static Synonym createConstant(const std::string& name) {
    return {DesignEntityType::CONSTANT, name};
}

// ai-gen start(copilot, 1, e)
// prompt: write test cases that creates QueryEntity objects for each entity type
// and checks that the type and name are correctly set

TEST_CASE("Creates QueryEntity for type variable1") {
    Synonym qe{createVariable("v")};
    REQUIRE(qe.getType() == DesignEntityType::VARIABLE);
    REQUIRE(qe.getName() == "v");
}

TEST_CASE("Creates QueryEntity for type procedure") {
    Synonym qe{createProcedure("p")};
    REQUIRE(qe.getType() == DesignEntityType::PROCEDURE);
    REQUIRE(qe.getName() == "p");
}

TEST_CASE("Creates QueryEntity for type constant") {
    Synonym qe{createConstant("c")};
    REQUIRE(qe.getType() == DesignEntityType::CONSTANT);
    REQUIRE(qe.getName() == "c");
}

// ai-gen end
