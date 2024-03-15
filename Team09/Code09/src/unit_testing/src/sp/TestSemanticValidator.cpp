
#include <memory>

#include "catch.hpp"
#include "sp/ast/SemanticValidator.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/de/AstVisitor.h"

using namespace std;  // NOLINT

TEST_CASE("Semantic Validation Tests") {
    SemanticValidator validator;
    auto xVar = std::make_shared<VariableNode>("x", 0);
    auto yVar = std::make_shared<VariableNode>("y", 1);
    auto printNode = std::make_shared<PrintNode>(yVar, 1);
    auto readNode = std::make_shared<ReadNode>(xVar, 0);
    std::vector<std::shared_ptr<StatementNode>> children = {readNode};
    std::vector<std::shared_ptr<StatementNode>> children1 = {printNode};
    auto stmtList1 = std::make_shared<StatementListNode>(children);
    auto stmtList2 = std::make_shared<StatementListNode>(children1);

    auto proc1 = std::make_shared<ProcedureNode>("a", stmtList1);
    auto proc2 = std::make_shared<ProcedureNode>("b", stmtList2);
    std::vector<std::shared_ptr<ProcedureNode>> children2 = {proc1, proc2};

    auto program = std::make_shared<ProgramNode>(children2);

    SECTION("Procedures with unique names has no error") {
        auto xVar = std::make_shared<VariableNode>("x", 0);
        auto yVar = std::make_shared<VariableNode>("y", 1);
        auto printNode = std::make_shared<PrintNode>(yVar, 1);
        auto readNode = std::make_shared<ReadNode>(xVar, 0);
        std::vector<std::shared_ptr<StatementNode>> children = {readNode};
        std::vector<std::shared_ptr<StatementNode>> children1 = {printNode};
        auto stmtList1 = std::make_shared<StatementListNode>(children);
        auto stmtList2 = std::make_shared<StatementListNode>(children1);

        auto proc1 = std::make_shared<ProcedureNode>("a", stmtList1);
        auto proc2 = std::make_shared<ProcedureNode>("b", stmtList2);
        std::vector<std::shared_ptr<ProcedureNode>> children2 = {proc1, proc2};

        auto program = std::make_shared<ProgramNode>(children2);
        REQUIRE_NOTHROW(validator.validateSemantics(program));
    }
    SECTION("Procedures with non-unique names throws") {
        auto xVar = std::make_shared<VariableNode>("x", 0);
        auto yVar = std::make_shared<VariableNode>("y", 1);
        auto printNode = std::make_shared<PrintNode>(yVar, 1);
        auto readNode = std::make_shared<ReadNode>(xVar, 0);
        std::vector<std::shared_ptr<StatementNode>> children = {readNode};
        std::vector<std::shared_ptr<StatementNode>> children1 = {printNode};
        auto stmtList1 = std::make_shared<StatementListNode>(children);
        auto stmtList2 = std::make_shared<StatementListNode>(children1);

        auto proc1 = std::make_shared<ProcedureNode>("a", stmtList1);
        auto proc2 = std::make_shared<ProcedureNode>("a", stmtList2);
        std::vector<std::shared_ptr<ProcedureNode>> children2 = {proc1, proc2};

        auto program = std::make_shared<ProgramNode>(children2);
        REQUIRE_THROWS_WITH(validator.validateSemantics(program), "Procedure with name: a declared twice");
    }
    SECTION("Calling procedure does not throw error") {
        auto callStatement = std::make_shared<CallNode>("a", 0);
        auto xVar = std::make_shared<VariableNode>("x", 0);
        auto readNode = std::make_shared<ReadNode>(xVar, 0);
        std::vector<std::shared_ptr<StatementNode>> children2 = {readNode};
        auto stmtList1 = std::make_shared<StatementListNode>(children2);
        std::vector<std::shared_ptr<StatementNode>> procBChildren = {callStatement};
        auto stmtList2 = std::make_shared<StatementListNode>(procBChildren);

        auto procedure1 = std::make_shared<ProcedureNode>("a", stmtList1);
        auto procedure2 = std::make_shared<ProcedureNode>("b", stmtList2);
        std::vector<std::shared_ptr<ProcedureNode>> children = {procedure1, procedure2};
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_NOTHROW(validator.validateSemantics(program));
    }

    SECTION("Calling non procedure throws error") {
        auto callStatement = std::make_shared<CallNode>("c", 0);
        auto xVar = std::make_shared<VariableNode>("x", 0);
        auto readNode = std::make_shared<ReadNode>(xVar, 0);
        std::vector<std::shared_ptr<StatementNode>> children2 = {readNode};
        auto stmtList1 = std::make_shared<StatementListNode>(children2);
        std::vector<std::shared_ptr<StatementNode>> procBChildren = {callStatement};
        auto stmtList2 = std::make_shared<StatementListNode>(procBChildren);
        auto procedure1 = std::make_shared<ProcedureNode>("a", stmtList1);
        auto procedure2 = std::make_shared<ProcedureNode>("b", stmtList2);
        std::vector<std::shared_ptr<ProcedureNode>> children = {procedure1, procedure2};
        auto program = std::make_shared<ProgramNode>(children);
        REQUIRE_THROWS_WITH(validator.validateSemantics(program), "Attempted to call: c, which is not a procedure.");
    }
}
