
#include <memory>

#include "catch.hpp"
#include "sp/ast/AstNode.h"
#include "sp/ast/SemanticValidator.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/de/AstVisitor.h"

using namespace std;  // NOLINT

TEST_CASE("Semantic Validation Tests") {
    SemanticValidator validator;
    SECTION("Procedures with unique names has no error") {
        auto procedure1 = std::make_shared<ProcedureNode>("a", std::vector<std::shared_ptr<ASTNode>>());
        auto procedure2 = std::make_shared<ProcedureNode>("b", std::vector<std::shared_ptr<ASTNode>>());
        std::vector<std::shared_ptr<ASTNode>> children = {};
        children.push_back(procedure1);
        children.push_back(procedure2);
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_NOTHROW(validator.validateSemantics(program));
    }
    SECTION("Procedures with non-unique names throws") {
        auto procedure1 = std::make_shared<ProcedureNode>("a", std::vector<std::shared_ptr<ASTNode>>());
        auto procedure2 = std::make_shared<ProcedureNode>("a", std::vector<std::shared_ptr<ASTNode>>());
        std::vector<std::shared_ptr<ASTNode>> children = {};
        children.push_back(procedure1);
        children.push_back(procedure2);
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_THROWS_WITH(validator.validateSemantics(program), "Procedure with name: a declared twice");
    }
    SECTION("Calling procedure does not throw error") {
        auto callStatement = std::make_shared<CallNode>("a", 0);
        std::vector<std::shared_ptr<ASTNode>> procBChildren = {callStatement};
        auto procedure1 = std::make_shared<ProcedureNode>("a", std::vector<std::shared_ptr<ASTNode>>());
        auto procedure2 = std::make_shared<ProcedureNode>("b", procBChildren);
        std::vector<std::shared_ptr<ASTNode>> children = {procedure1, procedure2};
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_NOTHROW(validator.validateSemantics(program));
    }

    SECTION("Calling non procedure throws error") {
        auto callStatement = std::make_shared<CallNode>("c", 0);
        std::vector<std::shared_ptr<ASTNode>> procBChildren = {callStatement};
        auto procedure1 = std::make_shared<ProcedureNode>("a", std::vector<std::shared_ptr<ASTNode>>());
        auto procedure2 = std::make_shared<ProcedureNode>("b", procBChildren);
        std::vector<std::shared_ptr<ASTNode>> children = {procedure1, procedure2};
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_THROWS_WITH(validator.validateSemantics(program), "Attempted to call: c, which is not a procedure.");
    }
}
