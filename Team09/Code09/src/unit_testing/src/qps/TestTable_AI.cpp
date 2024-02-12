#include "catch.hpp"
#include "qps/evaluator/Table.h"

// helper functions
static QueryEntity createVariable(const std::string& name) {
    return { EntityType::VARIABLE, name };
}

static QueryEntity createProcedure(const std::string& name) {
    return { EntityType::PROCEDURE, name };
}

static QueryEntity createConstant(const std::string& name) {
    return { EntityType::CONSTANT, name };
}

// ai-gen start(copilot, 2, e)
// prompt: Test Table class
TEST_CASE("Test Table extract results") {
    std::vector<QueryEntity> headers = {
        createVariable("v"),
        createConstant("c"),
        createProcedure("p"),
    };
    std::vector<Row> rows = { {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"} };
    Table table(headers, rows);

    SECTION("Test extractResults") {
        std::vector<std::string> results = table.extractResults(createVariable("v"));
        REQUIRE(results == std::vector<std::string>{"x", "y", "z"});
    }

    SECTION("Test isEmpty") {
        REQUIRE_FALSE(table.isEmpty());
    }
}
// ai-gen end

// ai-gen start(copilot, 2, e)
// prompt: try to extract a non-existent query entity. it should return an empty vector
TEST_CASE("Test Table extract results with non-existent query entity") {
    std::vector<QueryEntity> headers = {
       createVariable("v"),
       createConstant("c"),
       createProcedure("p"),
    };
    std::vector<Row> rows = { {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"} };
    Table table(headers, rows);

    std::vector<std::string> results = table.extractResults(createVariable("nonExistent"));
    REQUIRE(results == std::vector<std::string>{});
}
// ai-gen end
