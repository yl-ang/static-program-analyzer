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

// ai-gen start(copilot,0,e)
// prompt: use copilot
TEST_CASE("Test Table equality") {
    SECTION("Equal tables") {
        std::vector<Synonym> headers = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns = {{"x", "y", "z"}, {"4", "5", "testString"}};
        Table table1(headers, columns);
        Table table2(headers, columns);

        REQUIRE(table1 == table2);
    }

    SECTION("Equal tables, different column orders") {
        std::vector<Synonym> headers = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {{"x", "y", "z"}, {"4", "5", "testString"}};
        Table table1(headers, columns1);

        std::vector<ColumnData> columns2 = {{"z", "x", "y"}, {"testString", "4", "5"}};
        Table table2(headers, columns2);

        REQUIRE(table1 == table2);
    }

    SECTION("Equal tables, different header orders") {
        std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {{"x", "y", "z"}, {"4", "5", "testString"}};
        Table table1(headers1, columns1);

        std::vector<Synonym> headers2 = {createConstant("c"), createVariable("v")};
        std::vector<ColumnData> columns2 = {{"4", "5", "testString"}, {"x", "y", "z"}};
        Table table2(headers2, columns2);

        REQUIRE(table1 == table2);
    }

    SECTION("Unequal tables") {
        std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {{"x", "y", "z"}, {"4", "5", "testString"}};
        Table table1(headers1, columns1);

        std::vector<Synonym> headers2 = {createVariable("v"), createProcedure("p")};
        std::vector<ColumnData> columns2 = {{"x", "y", "z"}, {"Yishun", "Punggol", "Singapore"}};
        Table table2(headers2, columns2);

        REQUIRE_FALSE(table1 == table2);
    }

    SECTION("1 empty table") {
        std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {};
        Table emptyTable(headers1, columns1);

        std::vector<Synonym> headers2 = {createVariable("v"), createProcedure("p")};
        std::vector<ColumnData> columns2 = {{"x", "y", "z"}, {"Yishun", "Punggol", "Singapore"}};
        Table nonEmptyTable(headers2, columns2);

        REQUIRE_FALSE(emptyTable == nonEmptyTable);
    }

    SECTION("Both empty tables, same headers") {
        std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {};
        Table emptyTable1(headers1, columns1);

        std::vector<Synonym> headers2 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns2 = {};
        Table emptyTable2(headers2, columns2);

        REQUIRE(emptyTable1 == emptyTable2);
    }

    SECTION("Both empty tables, different headers") {
        std::vector<Synonym> headers1 = {createVariable("v"), createConstant("c")};
        std::vector<ColumnData> columns1 = {};
        Table emptyTable1(headers1, columns1);

        std::vector<Synonym> headers2 = {createVariable("v"), createProcedure("p")};
        std::vector<ColumnData> columns2 = {};
        Table emptyTable2(headers2, columns2);

        REQUIRE_FALSE(emptyTable1 == emptyTable2);
    }
}
// ai-gen end
