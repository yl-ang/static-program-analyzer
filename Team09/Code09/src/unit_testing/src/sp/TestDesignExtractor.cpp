#include "catch.hpp"
#include "sp/ast/AstNode.h"
#include "sp/de/DesignExtractor.h"

TEST_CASE("Design Extractor Tests") {
    DesignExtractor designExtractor;

    // x = y;
    std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
    std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
    std::vector<std::unique_ptr<ASTNode>> childrenA = {};
    childrenA.push_back(std::move(xNode));
    childrenA.push_back(std::move(yNode));
    auto equalNode = std::make_unique<ASTNode>("", "assign", std::move(childrenA));

    // read a;
    std::unique_ptr<ASTNode> aNode = std::make_unique<ASTNode>("a", "var");
    std::vector<std::unique_ptr<ASTNode>> childrenR = {};
    childrenR.push_back(std::move(aNode));
    auto readNode = std::make_unique<ASTNode>("", "read", std::move(childrenR));

    // print z;
    std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");
    std::vector<std::unique_ptr<ASTNode>> childrenP = {};
    childrenP.push_back(std::move(zNode));
    auto printNode = std::make_unique<ASTNode>("", "print", std::move(childrenP));

    std::vector<std::unique_ptr<ASTNode>> children1 = {};
    std::vector<std::unique_ptr<ASTNode>> children2 = {};

    children1.push_back(std::move(equalNode));
    auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(children1));

    children2.push_back(std::move(stmtList));
    auto procedure = ASTNode("a", "proc", std::move(children2));

    SECTION("Variables extracted correctly") {
        REQUIRE(expectedVariables == designExtractor.getVariables());
    }

    SECTION("Constants extracted correctly") {
        REQUIRE(expectedConstants == designExtractor.getConstants());
    }

    SECTION("Procedure extracted correctly") {
        REQUIRE(expectedProcedures == designExtractor.getProcedures());
    }

    SECTION("Statements extracted correctly") {
        REQUIRE(expectedStatements == designExtractor.getStatements());
    }

    SECTION("Follows extracted correctly") {
        REQUIRE(expectedFollows == designExtractor.getFollows());
    }

    SECTION("Parent extracted correctly") {
        REQUIRE(expectedParent == designExtractor.getParent());
    }

    SECTION("Uses extracted correctly") {
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("Modifies extracted correctly") {
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("Pattern extracted correctly") {
        REQUIRE(expectedPattern == designExtractor.getPattern());
    }
}
