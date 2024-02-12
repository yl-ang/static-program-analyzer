#include "catch.hpp"

#include "sp/ast/AstNode.h"
#include "sp/de/DesignExtractor.h"

TEST_CASE("Design Extractor Tests") {
    DesignExtractor designExtractor;

    SECTION("extract") {
        ASTNode root = ASTNode("+", "add");
        ASTNode child1 = ASTNode("x", "var");
        ASTNode child2 = ASTNode("*", "mul");
        ASTNode child3 = ASTNode("y", "var");
        ASTNode child4 = ASTNode("1", "const");
        ASTNode child5 = ASTNode("z", "var");
        ASTNode child6 = ASTNode("3", "const");

        child1.add_child(child3);
        child1.add_child(child4);
        child1.add_child(child5);
        child3.add_child(child6);
        root.add_child(child1);
        root.add_child(child2);

        designExtractor.extract(root);

        std::unordered_set<std::string> expectedVariables;
        expectedVariables.insert({"x", "y", "z"});

        REQUIRE(expectedVariables == designExtractor.getVariables());
    }
}
