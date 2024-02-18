#include <deque>
#include <iostream>

#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/ast/AstNode.h"
#include "sp/tokenizer/Token.h"

using namespace std;  // NOLINT

TEST_CASE("AST Build Tests") {
    AST ast;

    SECTION("Build variable node correctly") {
        // we are not interested in the types here so we just use a dummy type
        // similarly for line number.
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0)};
        ASTNode expectedNode = ASTNode("a", "var");

        ASTNode result = ast.buildVariableNameAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build constant node correctly") {
        // we are not interested in the types here so we just use a dummy type
        // similarly for line number.
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0)};
        ASTNode expectedNode = ASTNode("1", "const");

        ASTNode result = ast.buildConstAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build const factor ast correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0)};
        ASTNode expectedNode = ASTNode("1", "const");

        ASTNode result = ast.buildFactorAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build variable factor ast correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0)};
        ASTNode expectedNode = ASTNode("a", "var");

        ASTNode result = ast.buildFactorAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build assignment to const ast correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0)};

        ASTNode expectedNode = ASTNode("", "assign");
        ASTNode nameNode = ASTNode("a", "var");
        ASTNode integerNode = ASTNode("1", "const");

        expectedNode.add_child(nameNode);
        expectedNode.add_child(integerNode);

        ASTNode result = ast.buildAssignmentAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build assignment to var ast correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0)};

        ASTNode expectedNode = ASTNode("", "assign");
        ASTNode nameNode = ASTNode("a", "var");
        ASTNode integerNode = ASTNode("b", "var");

        expectedNode.add_child(nameNode);
        expectedNode.add_child(integerNode);

        ASTNode result = ast.buildAssignmentAST(inputTokenArray);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build adding two terms correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode constNode = ASTNode("b", "var");
        ASTNode const2Node = ASTNode("c", "var");

        addNode.add_child(constNode);
        addNode.add_child(const2Node);

        ASTNode result = ast.buildExpressionAST(inputTokenArray);
        REQUIRE(addNode == result);
    }

    SECTION("Build adding multiple terms correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode addNode2 = ASTNode("", "add");
        ASTNode zNode = ASTNode("z", "var");

        addNode2.add_child(xNode);
        addNode2.add_child(yNode);

        addNode.add_child(addNode2);
        addNode.add_child(zNode);

        ASTNode result = ast.buildExpressionAST(inputTokenArray);
        REQUIRE(addNode == result);
    }

    SECTION("Build multiplying two terms correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0)};

        ASTNode mulNode = ASTNode("", "mul");
        ASTNode xtNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");

        mulNode.add_child(xtNode);
        mulNode.add_child(yNode);

        ASTNode result = ast.buildExpressionAST(inputTokenArray);
        REQUIRE(mulNode == result);
    }

    SECTION("Build add then mul correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");

        mulNode.add_child(yNode);
        mulNode.add_child(zNode);

        addNode.add_child(xNode);
        addNode.add_child(mulNode);

        ASTNode result = ast.buildExpressionAST(inputTokenArray);
        REQUIRE(addNode == result);
    }

    SECTION("Build mul then add correctly") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");

        mulNode.add_child(xNode);
        mulNode.add_child(yNode);

        addNode.add_child(mulNode);
        addNode.add_child(zNode);

        ASTNode result = ast.buildExpressionAST(inputTokenArray);
        REQUIRE(addNode == result);
    }

    SECTION("Build assignment procedure") {
        std::deque<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 1),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 1),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };

        ASTNode procedure = ASTNode("a", "proc");
        ASTNode stmtList = ASTNode("", "stmtList");
        ASTNode equalNode = ASTNode("", "assign");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");

        equalNode.add_child(xNode);
        equalNode.add_child(yNode);

        stmtList.add_child(equalNode);
        procedure.add_child(stmtList);

        ASTNode result = ast.buildProcedureAST(inputTokenArray);
        REQUIRE(procedure == result);
    }
}
