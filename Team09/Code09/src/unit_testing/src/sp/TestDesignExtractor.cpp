/*
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

    // while ((c < d) && (e > f)) {
    //     g = h;
    // }
    std::unique_ptr<ASTNode> cNode = std::make_unique<ASTNode>("c", "var");
    std::unique_ptr<ASTNode> dNode = std::make_unique<ASTNode>("d", "var");
    std::unique_ptr<ASTNode> eNode = std::make_unique<ASTNode>("e", "var");
    std::unique_ptr<ASTNode> fNode = std::make_unique<ASTNode>("f", "var");
    std::unique_ptr<ASTNode> gNode = std::make_unique<ASTNode>("g", "var");
    std::unique_ptr<ASTNode> hNode = std::make_unique<ASTNode>("h", "var");

    std::vector<std::unique_ptr<ASTNode>> childrenW = {};
    std::vector<std::unique_ptr<ASTNode>> childrenW2 = {};
    std::vector<std::unique_ptr<ASTNode>> childrenW3 = {};
    std::vector<std::unique_ptr<ASTNode>> childrenW4 = {};
    std::vector<std::unique_ptr<ASTNode>> childrenW5 = {};
    std::vector<std::unique_ptr<ASTNode>> childrenW6 = {};

    childrenW.push_back(std::move(cNode));
    childrenW.push_back(std::move(dNode));

    auto lessThan = std::make_unique<ASTNode>("", "<", std::move(childrenW));
    childrenW2.push_back(std::move(eNode));
    childrenW2.push_back(std::move(fNode));

    auto moreThan = std::make_unique<ASTNode>("", ">", std::move(childrenW2));
    childrenW3.push_back(std::move(lessThan));
    childrenW3.push_back(std::move(moreThan));
    auto andandNode = std::make_unique<ASTNode>("", "&&", std::move(childrenW3));

    childrenW4.push_back(std::move(gNode));
    childrenW4.push_back(std::move(hNode));

    auto assign = std::make_unique<ASTNode>("", "assign", std::move(childrenW4));
    childrenW6.push_back(std::move(assign));
    auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(childrenW6));

    childrenW5.push_back(std::move(andandNode));
    childrenW5.push_back(std::move(stmtList));

    auto whileNode = std::make_unique<ASTNode>("", "while", std::move(childrenW5));

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
*/
