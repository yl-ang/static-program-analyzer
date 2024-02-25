#include <iostream>
#include <queue>

#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/ast/AstNode.h"
#include "sp/tokenizer/Token.h"

using namespace std;  // NOLINT

std::queue<Token> makeTokenQueue(std::vector<Token> tokens) {
    std::queue<Token> queue;
    for (auto token : tokens) {
        queue.push(token);
    }
    return queue;
}

TEST_CASE("AST Build Tests") {
    AST ast;

    SECTION("Build const factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0)};
        ASTNode expectedNode = ASTNode("1", "const");

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0)};
        ASTNode expectedNode = ASTNode("a", "var");
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build assignment to const ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode expectedNode = ASTNode("", "assign");
        ASTNode nameNode = ASTNode("a", "var");
        ASTNode integerNode = ASTNode("1", "const");

        expectedNode.add_child(nameNode);
        expectedNode.add_child(integerNode);

        ASTNode result = ast.buildAssignmentAST(queue);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build assignment to var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0), Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0), Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1)};

        ASTNode expectedNode = ASTNode("", "assign");
        ASTNode nameNode = ASTNode("a", "var");
        ASTNode integerNode = ASTNode("b", "var");

        expectedNode.add_child(nameNode);
        expectedNode.add_child(integerNode);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildAssignmentAST(queue);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build adding two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
                                              Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode constNode = ASTNode("b", "var");
        ASTNode const2Node = ASTNode("c", "var");

        addNode.add_child(constNode);
        addNode.add_child(const2Node);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build adding multiple terms correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
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
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build multiplying two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
                                              Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0)};

        ASTNode mulNode = ASTNode("", "mul");
        ASTNode xtNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");

        mulNode.add_child(xtNode);
        mulNode.add_child(yNode);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildTermAST(queue);
        REQUIRE(mulNode == result);
    }

    SECTION("Build add then mul correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
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
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
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
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build assignment procedure") {
        std::vector<Token> inputTokenArray = {
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
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildProcedureAST(queue);
        REQUIRE(procedure == result);
    }

    SECTION("Build read statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::READ, "read", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };

        ASTNode expectedNode = ASTNode("", "read");
        ASTNode nameNode = ASTNode("a", "var");

        expectedNode.add_child(nameNode);
        auto queue = makeTokenQueue(inputTokenArray);
        ASTNode result = ast.buildReadAST(queue);
        REQUIRE(expectedNode == result);
    }
    SECTION("Build read procedure correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::READ, "read", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 1),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };

        ASTNode procedure = ASTNode("a", "proc");
        ASTNode stmtList = ASTNode("", "stmtList");
        ASTNode readNode = ASTNode("", "read");
        ASTNode nameNode = ASTNode("y", "var");

        readNode.add_child(nameNode);

        stmtList.add_child(readNode);
        procedure.add_child(stmtList);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildProcedureAST(queue);
        REQUIRE(procedure == result);
    }

    SECTION("Build print statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PRINT, "print", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };

        ASTNode expectedNode = ASTNode("", "print");
        ASTNode nameNode = ASTNode("a", "var");

        expectedNode.add_child(nameNode);
        auto queue = makeTokenQueue(inputTokenArray);
        ASTNode result = ast.buildPrintAST(queue);
        REQUIRE(expectedNode == result);
    }
    SECTION("Build print procedure correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::PRINT, "print", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 1),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };

        ASTNode procedure = ASTNode("a", "proc");
        ASTNode stmtList = ASTNode("", "stmtList");
        ASTNode readNode = ASTNode("", "print");
        ASTNode nameNode = ASTNode("y", "var");

        readNode.add_child(nameNode);

        stmtList.add_child(readNode);
        procedure.add_child(stmtList);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildProcedureAST(queue);
        REQUIRE(procedure == result);
    }

    SECTION("Build parenthesised expressions ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode constNode = ASTNode("b", "var");
        ASTNode const2Node = ASTNode("c", "var");

        addNode.add_child(constNode);
        addNode.add_child(const2Node);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(addNode == result);
    }
    SECTION("Build parenthesised expressions with multiple parenthesis ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode constNode = ASTNode("b", "var");
        ASTNode const2Node = ASTNode("c", "var");

        addNode.add_child(constNode);
        addNode.add_child(const2Node);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build parenthesised mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");

        mulNode.add_child(xNode);
        mulNode.add_child(yNode);

        addNode.add_child(mulNode);
        addNode.add_child(zNode);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build multiple parenthesised mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");

        mulNode.add_child(xNode);
        mulNode.add_child(yNode);

        addNode.add_child(mulNode);
        addNode.add_child(zNode);
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(addNode == result);
    }

    SECTION("Build parenthesised const factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        ASTNode expectedNode = ASTNode("1", "const");

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == result);
    }
    SECTION("Build parenthesised variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        ASTNode expectedNode = ASTNode("a", "var");
        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == result);
    }

    SECTION("Build parenthesis first then var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");

        addNode.add_child(xNode);
        addNode.add_child(yNode);
        mulNode.add_child(addNode);
        mulNode.add_child(zNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(mulNode == result);
    }

    SECTION("Build var first then parenthesis ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),         Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0), Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),          Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};

        ASTNode addNode = ASTNode("", "add");
        ASTNode bNode = ASTNode("b", "var");
        ASTNode cNode = ASTNode("c", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode aNode = ASTNode("a", "var");

        addNode.add_child(bNode);
        addNode.add_child(cNode);
        mulNode.add_child(aNode);
        mulNode.add_child(addNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(mulNode == result);
    }

    SECTION("Build var first then parenthesis then var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),          Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
        };

        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode zNode = ASTNode("z", "var");
        ASTNode zNode2 = ASTNode("z", "var");
        ASTNode addNode2 = ASTNode("", "add");

        addNode.add_child(xNode);
        addNode.add_child(yNode);
        mulNode.add_child(zNode);
        mulNode.add_child(addNode);
        addNode2.add_child(mulNode);
        addNode2.add_child(zNode2);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildExpressionAST(queue);
        REQUIRE(addNode2 == result);
    }

    SECTION("Build rel_factors correctly for var") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        };
        ASTNode zNode = ASTNode("a", "var");
        auto queue = makeTokenQueue(inputTokenArray);
        ASTNode result = ast.buildRelationalFactorAST(queue);
        REQUIRE(zNode == result);
    }

    SECTION("Build rel_factors correctly for const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
        };
        ASTNode oneNode = ASTNode("1", "const");
        auto queue = makeTokenQueue(inputTokenArray);
        ASTNode result = ast.buildRelationalFactorAST(queue);
        REQUIRE(oneNode == result);
    }
    SECTION("Build rel_factors correctly for expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
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
        auto queue = makeTokenQueue(inputTokenArray);
        ASTNode result = ast.buildRelationalFactorAST(queue);
        REQUIRE(result == result);
    }

    SECTION("Build relational expression with var and const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
        };

        ASTNode zNode = ASTNode("z", "var");
        ASTNode oneNode = ASTNode("1", "const");
        ASTNode lessThan = ASTNode("", "<");

        lessThan.add_child(zNode);
        lessThan.add_child(oneNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }
    SECTION("Build relational expression with var and var") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "y", 0),
        };

        ASTNode zNode = ASTNode("z", "var");
        ASTNode yNode = ASTNode("y", "const");
        ASTNode lessThan = ASTNode("", "<");

        lessThan.add_child(zNode);
        lessThan.add_child(yNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }

    SECTION("Build relational expression with const and var") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        ASTNode zNode = ASTNode("z", "var");
        ASTNode oneNode = ASTNode("1", "const");
        ASTNode lessThan = ASTNode("", "<");

        lessThan.add_child(oneNode);
        lessThan.add_child(zNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }

    SECTION("Build relational expression with const and const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "2", 0),
        };

        ASTNode twoNode = ASTNode("2", "var");
        ASTNode oneNode = ASTNode("1", "const");
        ASTNode lessThan = ASTNode("", "<");

        lessThan.add_child(oneNode);
        lessThan.add_child(twoNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }

    SECTION("Build relational expression with var and expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        ASTNode zNode = ASTNode("z", "var");
        ASTNode lessThan = ASTNode("", "<");
        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        addNode.add_child(xNode);
        addNode.add_child(yNode);

        lessThan.add_child(zNode);
        lessThan.add_child(addNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }

    SECTION("Build relational expression with expressions") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode lessThan = ASTNode("", "<");
        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode2 = ASTNode("x", "var");
        ASTNode yNode2 = ASTNode("y", "var");
        addNode.add_child(xNode2);
        addNode.add_child(yNode2);

        mulNode.add_child(xNode);
        mulNode.add_child(yNode);

        lessThan.add_child(mulNode);
        lessThan.add_child(addNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }

    SECTION("Build relational expression with parenthesised exprs") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0), Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),          Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),         Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        ASTNode xNode = ASTNode("x", "var");
        ASTNode yNode = ASTNode("y", "var");
        ASTNode mulNode = ASTNode("", "mul");
        ASTNode lessThan = ASTNode("", "<");
        ASTNode addNode = ASTNode("", "add");
        ASTNode xNode2 = ASTNode("x", "var");
        ASTNode yNode2 = ASTNode("y", "var");
        addNode.add_child(xNode2);
        addNode.add_child(yNode2);

        mulNode.add_child(xNode);
        mulNode.add_child(yNode);

        lessThan.add_child(mulNode);
        lessThan.add_child(addNode);

        auto queue = makeTokenQueue(inputTokenArray);

        ASTNode result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == result);
    }
}
