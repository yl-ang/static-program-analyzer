#include <algorithm>

#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/evaluator/TableManager.h"

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

TEST_CASE("Test TableManager join") {
    SECTION("Join table onto empty table manager") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        std::vector<ColumnData> columns = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table(headers, columns);

        TableManager tableManager{};
        tableManager.join(table);
        Table result = tableManager.getTable();

        REQUIRE(result == table);
    }

    SECTION("Joins 2 tables") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        std::vector<ColumnData> columns = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table(headers, columns);

        TableManager tableManager(table);
        Table table2(headers, columns);

        tableManager.join(table2);
        Table result = tableManager.getTable();

        REQUIRE(result == table2);
    }

    SECTION("Joins 2 different tables") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns1 = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table1(headers, columns1);
        tableManager.join(table1);

        std::vector<ColumnData> columns2 = {
            {"x", "y", "a"}, {"4", "5", "badMatch"}, {"Yishun", "Punggol", "Singapore"}};
        Table table2(headers, columns2);
        tableManager.join(table2);

        Table result = tableManager.getTable();

        std::vector<ColumnData> expectedColumns = {{"x", "y"}, {"4", "5"}, {"Yishun", "Punggol"}};
        Table expected = Table(headers, expectedColumns);

        REQUIRE(result == expected);
    }

    SECTION("Joins 3 tables") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns1 = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table1(headers, columns1);
        tableManager.join(table1);

        std::vector<ColumnData> columns2 = {{"x", "y"}, {"4", "5"}, {"Yishun", "Punggol"}};
        Table table2(headers, columns2);
        tableManager.join(table2);

        std::vector<ColumnData> columns3 = {{"a", "y", "z"}, {"4", "5", "testString"}, {"LOL", "Punggol", "Singapore"}};
        Table table3(headers, columns3);
        tableManager.join(table3);

        Table result = tableManager.getTable();

        std::vector<ColumnData> expectedColumns = {{"y"}, {"5"}, {"Punggol"}};
        Table expected = Table(headers, expectedColumns);

        REQUIRE(result == expected);
    }

    SECTION("Join empty table to full table") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns1 = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table1(headers, columns1);
        tableManager.join(table1);

        std::vector<ColumnData> emptyColumns{};
        Table table2{{createVariable("v1")}, emptyColumns};
        tableManager.join(table2);

        Table result = tableManager.getTable();

        std::vector<Synonym> expectedHeaders = {createVariable("v"), createConstant("c"), createProcedure("p"),
                                                createVariable("v1")};
        Table expected = Table(expectedHeaders, emptyColumns);

        REQUIRE(result == expected);
    }

    SECTION("Join 2 tables with different headers") {
        std::vector<Synonym> headers1 = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        std::vector<ColumnData> columns1 = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table1(headers1, columns1);

        std::vector<Synonym> headers2 = {
            createVariable("v1"),
            createConstant("c1"),
            createProcedure("p1"),
        };

        std::vector<ColumnData> columns2 = {
            {"a", "b", "c"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table2(headers2, columns2);

        TableManager tableManager{table1};
        tableManager.join(table2);
        Table result = tableManager.getTable();

        std::vector<Synonym> expectedHeaders = {createVariable("v"),  createConstant("c"),  createProcedure("p"),
                                                createVariable("v1"), createConstant("c1"), createProcedure("p1")};
        // create expected columns by doing a cartesian product of columns1 and columns2
        std::vector<ColumnData> expectedColumns{
            {"x", "x", "x", "y", "y", "y", "z", "z", "z"},
            {"4", "4", "4", "5", "5", "5", "testString", "testString", "testString"},
            {"Yishun", "Yishun", "Yishun", "Punggol", "Punggol", "Punggol", "Singapore", "Singapore", "Singapore"},
            {"a", "b", "c", "a", "b", "c", "a", "b", "c"},
            {"4", "5", "testString", "4", "5", "testString", "4", "5", "testString"},
            {"Yishun", "Punggol", "Singapore", "Yishun", "Punggol", "Singapore", "Yishun", "Punggol", "Singapore"}};

        Table expected = Table(expectedHeaders, expectedColumns);

        REQUIRE(result == expected);
    }
}

TEST_CASE("Test joinAll") {
    SECTION("Joins vector of tables") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns1 = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table1(headers, columns1);
        tableManager.join(table1);

        std::vector<ColumnData> columns2 = {{"x", "y"}, {"4", "5"}, {"Yishun", "Punggol"}};
        Table table2(headers, columns2);

        std::vector<ColumnData> columns3 = {{"a", "y", "z"}, {"4", "5", "testString"}, {"LOL", "Punggol", "Singapore"}};
        Table table3(headers, columns3);

        std::vector<Table> tables = {table2, table3};
        tableManager.joinAll(tables);

        Table result = tableManager.getTable();

        std::vector<ColumnData> expectedColumns = {{"y"}, {"5"}, {"Punggol"}};
        Table expected = Table(headers, expectedColumns);

        REQUIRE(result == expected);
    }
}

TEST_CASE("Test extractResults") {
    SECTION("Extracts results from table") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table(headers, columns);
        tableManager.join(table);

        std::vector<Synonym> synonyms = {createVariable("v")};
        std::vector<std::string> result = tableManager.extractResults(synonyms);

        std::vector<std::string> expected = {"x", "y", "z"};
        REQUIRE(result == expected);
    }

    SECTION("Extracts results from empty table") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns{};
        Table table(headers, columns);
        tableManager.join(table);

        std::vector<Synonym> synonyms = {createVariable("v"), createConstant("c")};
        std::vector<std::string> result = tableManager.extractResults(synonyms);

        std::vector<std::string> expected{};
        REQUIRE(result == expected);
    }

    SECTION("Extracts multi-results from table") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table(headers, columns);
        tableManager.join(table);

        std::vector<Synonym> synonyms = {createVariable("v"), createConstant("c")};
        std::vector<std::string> result = tableManager.extractResults(synonyms);

        std::vector<std::string> expected = {"x 4", "y 5", "z testString"};
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(result == expected);
    }

    SECTION("No results if multi-select is not subset of table headers") {
        std::vector<Synonym> headers = {
            createVariable("v"),
            createConstant("c"),
            createProcedure("p"),
        };

        TableManager tableManager{};

        std::vector<ColumnData> columns = {
            {"x", "y", "z"}, {"4", "5", "testString"}, {"Yishun", "Punggol", "Singapore"}};
        Table table(headers, columns);
        tableManager.join(table);

        std::vector<Synonym> synonyms = {createVariable("v"), createConstant("c"), createVariable("v1")};
        std::vector<std::string> result = tableManager.extractResults(synonyms);

        std::vector<std::string> expected{};
        REQUIRE(result == expected);
    }
}
