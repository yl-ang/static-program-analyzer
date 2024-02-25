#include "PKB/Stores/StatementStore.h"
#include "catch.hpp"

TEST_CASE("StatementStore tests", "[StatementStore]") {
    // Test case 1: Test getStatements
    SECTION("Test getStatements") {
        StatementStore statementStore;
        std::unordered_set<Stmt> inputStmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::IF, 2}};
        statementStore.setStatements(inputStmts);

        const auto& result = statementStore.getStatements();
        REQUIRE(result == inputStmts);
    }

    SECTION("Test getStatementByStmtNum") {
        StatementStore statementStore;
        std::unordered_set<Stmt> inputStmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::IF, 2}};
        statementStore.setStatements(inputStmts);

        // Test if statements are set correctly
        const auto& result1 = statementStore.getStatements();
        REQUIRE(result1 == inputStmts);

        // Test getStatementByStmtNum
        REQUIRE(statementStore.getStatementByStmtNum(1).value() == Stmt{StatementType::ASSIGN, 1});
        REQUIRE(statementStore.getStatementByStmtNum(2).value() == Stmt{StatementType::IF, 2});
        REQUIRE_FALSE(statementStore.getStatementByStmtNum(3).has_value());  // Non-existing statement
    }

    SECTION("Test getStatementsByType") {
        StatementStore statementStore;
        std::unordered_set<Stmt> inputStmts = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::IF, 2},
                                               Stmt{StatementType::WHILE, 3}};
        statementStore.setStatements(inputStmts);

        // Test getStatementsByType
        auto result = statementStore.getStatementsByType(StatementType::ASSIGN);
        REQUIRE(!result.empty());
        REQUIRE(result == std::unordered_set<std::optional<Stmt>>{std::make_optional(Stmt{StatementType::ASSIGN, 1})});

        result = statementStore.getStatementsByType(StatementType::READ);
        REQUIRE_FALSE(!result.empty());
    }
}
