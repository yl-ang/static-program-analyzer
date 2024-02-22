#include "catch.hpp"
#include "qps/evaluator/Table.h"

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

// ai-gen start(copilot, 2, e)
// prompt: Test Table class
TEST_CASE("Test Table extract results") {
    std::vector<Synonym> headers = {
        createVariable("v"),
        createConstant("c"),
        createProcedure("p"),
    };

    std::vector<ColumnData> columns = {{"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
    Table table(headers, columns);

    SECTION("Test extractResults") {
        std::vector<std::string> results = table.extractResults({createVariable("v")});
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
    std::vector<Synonym> headers = {
        createVariable("v"),
        createConstant("c"),
        createProcedure("p"),
    };
    std::vector<ColumnData> rows = {{"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
    Table table(headers, rows);

    std::vector<std::string> results = table.extractResults({createVariable("nonExistent")});
    REQUIRE(results == std::vector<std::string>{});
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: test case for Table join
TEST_CASE("Test Table join") {
    std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
    std::vector<ColumnData> columns1 = {{"x", "y", "z"}, {"4", "5", "testString"}};
    Table table1(headers1, columns1);

    std::vector<Synonym> headers2 = {createVariable("v"), createProcedure("p")};
    std::vector<ColumnData> columns2 = {{"x", "y", "z"}, {"Yishun", "Punggol", "Singapore"}};
    Table table2(headers2, columns2);

    Table joinedTable = table1.join(table2);

    std::vector<Synonym> expectedHeaders = {createVariable("v"), createConstant("c"), createProcedure("p")};
    std::vector<ColumnData> expectedColumns = {
        {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
    Table expectedTable(expectedHeaders, expectedColumns);

    REQUIRE(joinedTable.extractResults({createVariable("v")}) == expectedTable.extractResults({createVariable("v")}));
    REQUIRE(joinedTable.extractResults({createConstant("c")}) == expectedTable.extractResults({createConstant("c")}));
    REQUIRE(joinedTable.extractResults({createProcedure("p")}) == expectedTable.extractResults({createProcedure("p")}));
}
// ai-gen end

// ai-gen start(copilot, 1, e)
// prompt: test case for table join where the tables have no common headers
TEST_CASE("Test Table join with no common headers") {
    std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
    std::vector<ColumnData> columns1 = {{"x", "y", "z"}, {"4", "5", "testString"}};
    Table table1(headers1, columns1);

    std::vector<Synonym> headers2 = {createProcedure("p"), createProcedure("p1")};
    std::vector<ColumnData> columns2 = {{"Yishun", "Punggol", "Singapore"}, {"Sengkang", "Jurong", "Serangoon"}};
    Table table2(headers2, columns2);

    Table joinedTable = table1.join(table2);

    std::vector<Synonym> expectedHeaders = {createVariable("v"), createConstant("c"), createProcedure("p"),
                                            createProcedure("p1")};
    std::vector<ColumnData> expectedColumns = {{"x", "y", "z"},
                                               {"4", "5", "testString"},
                                               {"Yishun", "Punggol", "Singapore"},
                                               {"Sengkang", "Jurong", "Serangoon"}};
    Table expectedTable(expectedHeaders, expectedColumns);

    // check expected headers
    REQUIRE(joinedTable.getHeaders() == expectedHeaders);

    REQUIRE(joinedTable.extractResults({createVariable("v")}) == expectedTable.extractResults({createVariable("v")}));
    REQUIRE(joinedTable.extractResults({createConstant("c")}) == expectedTable.extractResults({createConstant("c")}));
    REQUIRE(joinedTable.extractResults({createProcedure("p")}) == expectedTable.extractResults({createProcedure("p")}));
    REQUIRE(joinedTable.extractResults({createProcedure("p1")}) ==
            expectedTable.extractResults({createProcedure("p1")}));
}
// ai-gen end

// prompt: test case for table join where the tables have common headers. However, not all the rows have the same values
// for the common headers
TEST_CASE("Test Table join with common headers but not all rows have the same values") {
    std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
    std::vector<ColumnData> columns1 = {{"x", "z", "l"}, {"4", "5", "testString"}};
    Table table1(headers1, columns1);

    std::vector<Synonym> headers2 = {createVariable("v"), createProcedure("p")};
    std::vector<ColumnData> columns2 = {{"x", "y", "k", "z"}, {"Sengkang", "Jurong", "Serangoon", "Woodlands"}};
    Table table2(headers2, columns2);

    Table joinedTable = table1.join(table2);

    std::vector<Synonym> expectedHeaders = {createVariable("v"), createConstant("c"), createProcedure("p")};
    std::vector<ColumnData> expectedColumns = {{"x", "z"}, {"4", "5"}, {"Sengkang", "Woodlands"}};
    Table expectedTable(expectedHeaders, expectedColumns);

    // check expected headers
    REQUIRE(joinedTable.getHeaders() == expectedHeaders);

    REQUIRE(joinedTable.extractResults({createVariable("v")}) == expectedTable.extractResults({createVariable("v")}));
    REQUIRE(joinedTable.extractResults({createConstant("c")}) == expectedTable.extractResults({createConstant("c")}));
    REQUIRE(joinedTable.extractResults({createConstant("p")}) == expectedTable.extractResults({createConstant("p")}));
}
// ai-gen end
