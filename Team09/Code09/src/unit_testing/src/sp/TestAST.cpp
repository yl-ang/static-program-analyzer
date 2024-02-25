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

        auto result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0)};
        ASTNode expectedNode = ASTNode("a", "var");
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build assignment to const ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };
        auto queue = makeTokenQueue(inputTokenArray);

        std::unique_ptr<ASTNode> nameNode = std::make_unique<ASTNode>("a", "var");
        std::unique_ptr<ASTNode> integerNode = std::make_unique<ASTNode>("1", "const");

        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(nameNode));
        children.push_back(std::move(integerNode));

        ASTNode expectedNode = ASTNode("", "assign", std::move(children));

        auto result = ast.buildAssignmentAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build assignment to var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0), Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0), Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildAssignmentAST(queue);  // Do this first

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::unique_ptr<ASTNode> nameNode = std::make_unique<ASTNode>("a", "var");
        std::unique_ptr<ASTNode> integerNode = std::make_unique<ASTNode>("b", "var");

        // Populating the children _after_ running the method
        children.push_back(std::move(nameNode));
        children.push_back(std::move(integerNode));

        // Now we can create the expectedNode as children has been fully constructed.
        ASTNode expectedNode = ASTNode("", "assign", std::move(children));

        REQUIRE(expectedNode == *(result.get()));  // Now this should work
    }

    SECTION("Build adding two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
                                              Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0)};
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> constNode = std::make_unique<ASTNode>("b", "var");
        std::unique_ptr<ASTNode> const2Node = std::make_unique<ASTNode>("c", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(constNode));
        children.push_back(std::move(const2Node));

        auto addNode = ASTNode("", "add", std::move(children));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build adding multiple terms correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));

        children1.push_back(std::move(addNode));
        children1.push_back(std::move(zNode));

        auto addNode2 = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode2 == *(result.get()));
    }

    SECTION("Build multiplying two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
                                              Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildTermAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xtNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(xtNode));
        children.push_back(std::move(yNode));

        auto mulNode = ASTNode("", "mul", std::move(children));

        REQUIRE(mulNode == *(result.get()));
    }

    SECTION("Build add then mul correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(yNode));
        children.push_back(std::move(zNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children));
        children1.push_back(std::move(xNode));
        children1.push_back(std::move(mulNode));

        auto addNode = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children));
        children1.push_back(std::move(mulNode));

        children1.push_back(std::move(zNode));

        auto addNode = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode == *(result.get()));
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

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildProcedureAST(queue);
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));
        auto equalNode = std::make_unique<ASTNode>("", "assign", std::move(children));

        children1.push_back(std::move(equalNode));
        auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(children1));

        children2.push_back(std::move(stmtList));
        auto procedure = ASTNode("a", "proc", std::move(children2));

        REQUIRE(procedure == *(result.get()));
    }

    SECTION("Build read statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::READ, "read", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildReadAST(queue);

        std::unique_ptr<ASTNode> aNode = std::make_unique<ASTNode>("a", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(aNode));
        auto expectedNode = ASTNode("", "read", std::move(children));

        REQUIRE(expectedNode == *(result.get()));
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

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildProcedureAST(queue);
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};

        children.push_back(std::move(yNode));
        auto readNode = std::make_unique<ASTNode>("", "read", std::move(children));

        children1.push_back(std::move(readNode));
        auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(children1));

        children2.push_back(std::move(stmtList));
        auto procedure = ASTNode("a", "proc", std::move(children2));

        REQUIRE(procedure == *(result.get()));
    }

    SECTION("Build print statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PRINT, "print", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildPrintAST(queue);

        std::unique_ptr<ASTNode> aNode = std::make_unique<ASTNode>("a", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(aNode));
        auto expectedNode = ASTNode("", "print", std::move(children));

        REQUIRE(expectedNode == *(result.get()));
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

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildProcedureAST(queue);
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};

        children.push_back(std::move(yNode));
        auto printNode = std::make_unique<ASTNode>("", "print", std::move(children));

        children1.push_back(std::move(printNode));
        auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(children1));

        children2.push_back(std::move(stmtList));
        auto procedure = ASTNode("a", "proc", std::move(children2));

        REQUIRE(procedure == *(result.get()));
    }

    SECTION("Build parenthesised expressions ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildFactorAST(queue);  // Do this first
        std::unique_ptr<ASTNode> constNode = std::make_unique<ASTNode>("b", "var");
        std::unique_ptr<ASTNode> const2Node = std::make_unique<ASTNode>("c", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(constNode));
        children.push_back(std::move(const2Node));

        auto addNode = ASTNode("", "add", std::move(children));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build parenthesised expressions with multiple parenthesis ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildFactorAST(queue);  // Do this first
        std::unique_ptr<ASTNode> constNode = std::make_unique<ASTNode>("b", "var");
        std::unique_ptr<ASTNode> const2Node = std::make_unique<ASTNode>("c", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(constNode));
        children.push_back(std::move(const2Node));

        auto addNode = ASTNode("", "add", std::move(children));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build parenthesised mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildFactorAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children));
        children1.push_back(std::move(mulNode));

        children1.push_back(std::move(zNode));

        auto addNode = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode == *(result.get()));
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

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildFactorAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children));
        children1.push_back(std::move(mulNode));

        children1.push_back(std::move(zNode));

        auto addNode = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build parenthesised const factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);
        ASTNode expectedNode = ASTNode("1", "const");

        REQUIRE(expectedNode == *(result.get()));
    }
    SECTION("Build parenthesised variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);

        ASTNode expectedNode = ASTNode("a", "var");

        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build parenthesis first then var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));
        children1.push_back(std::move(addNode));

        children1.push_back(std::move(zNode));

        auto mulNode = ASTNode("", "mul", std::move(children1));

        REQUIRE(mulNode == *(result.get()));
    }

    SECTION("Build var first then parenthesis ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),         Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0), Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),          Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> aNode = std::make_unique<ASTNode>("a", "var");
        std::unique_ptr<ASTNode> bNode = std::make_unique<ASTNode>("b", "var");
        std::unique_ptr<ASTNode> cNode = std::make_unique<ASTNode>("c", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(bNode));
        children.push_back(std::move(cNode));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));
        children1.push_back(std::move(aNode));

        children1.push_back(std::move(addNode));

        auto mulNode = ASTNode("", "mul", std::move(children1));

        REQUIRE(mulNode == *(result.get()));
    }

    SECTION("Build var first then parenthesis then var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),          Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");
        std::unique_ptr<ASTNode> zNode2 = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));
        children1.push_back(std::move(zNode));
        children1.push_back(std::move(addNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children1));
        children2.push_back(std::move(mulNode));
        children2.push_back(std::move(zNode2));

        auto addNode2 = ASTNode("", "add", std::move(children2));

        REQUIRE(addNode2 == *(result.get()));
    }

    SECTION("Build rel_factors correctly for var") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);
        ASTNode aNode = ASTNode("a", "var");

        REQUIRE(aNode == *(result.get()));
    }

    SECTION("Build rel_factors correctly for const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);
        ASTNode oneNode = ASTNode("1", "const");

        REQUIRE(oneNode == *(result.get()));
    }
    SECTION("Build rel_factors correctly for expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);  // Do this first
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(yNode));
        children.push_back(std::move(zNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children));
        children1.push_back(std::move(xNode));
        children1.push_back(std::move(mulNode));

        auto addNode = ASTNode("", "add", std::move(children1));

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build relational expression with var and const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");
        std::unique_ptr<ASTNode> oneNode = std::make_unique<ASTNode>("1", "const");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(zNode));
        children.push_back(std::move(oneNode));
        auto lessThan = ASTNode("", "<", std::move(children));

        REQUIRE(lessThan == *(result.get()));
    }
    SECTION("Build relational expression with var and var") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(zNode));
        children.push_back(std::move(yNode));
        auto lessThan = ASTNode("", "<", std::move(children));

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with const and var") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");
        std::unique_ptr<ASTNode> oneNode = std::make_unique<ASTNode>("1", "const");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(oneNode));
        children.push_back(std::move(zNode));

        auto lessThan = ASTNode("", "<", std::move(children));

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with const and const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "2", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        std::unique_ptr<ASTNode> twoNode = std::make_unique<ASTNode>("2", "var");
        std::unique_ptr<ASTNode> oneNode = std::make_unique<ASTNode>("1", "const");
        std::vector<std::unique_ptr<ASTNode>> children = {};

        children.push_back(std::move(oneNode));
        children.push_back(std::move(twoNode));

        auto lessThan = ASTNode("", "<", std::move(children));

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with var and expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> zNode = std::make_unique<ASTNode>("z", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));
        children1.push_back(std::move(zNode));
        children1.push_back(std::move(addNode));

        auto lessThan = ASTNode("", "<", std::move(children1));
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with expressions") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode2 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode2 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        children.push_back(std::move(xNode2));
        children.push_back(std::move(yNode2));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));

        children1.push_back(std::move(xNode));
        children1.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children1));

        children2.push_back(std::move(mulNode));
        children2.push_back(std::move(addNode));

        auto lessThan = ASTNode("", "<", std::move(children2));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with parenthesised exprs") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),          Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode2 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode2 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        children.push_back(std::move(xNode2));
        children.push_back(std::move(yNode2));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));

        children1.push_back(std::move(xNode));
        children1.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children1));

        children2.push_back(std::move(mulNode));
        children2.push_back(std::move(addNode));

        auto lessThan = ASTNode("", "<", std::move(children2));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build conditional expr to bracketed rel_expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),          Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode2 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode2 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        children.push_back(std::move(xNode2));
        children.push_back(std::move(yNode2));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));

        children1.push_back(std::move(xNode));
        children1.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children1));

        children2.push_back(std::move(mulNode));
        children2.push_back(std::move(addNode));

        auto lessThan = ASTNode("", "<", std::move(children2));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildConditionalExpressionAST(queue);
        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build conditional expr to non bracketed rel_expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),           Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),          Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode2 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode2 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children1 = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        children.push_back(std::move(xNode2));
        children.push_back(std::move(yNode2));

        auto addNode = std::make_unique<ASTNode>("", "add", std::move(children));

        children1.push_back(std::move(xNode));
        children1.push_back(std::move(yNode));

        auto mulNode = std::make_unique<ASTNode>("", "mul", std::move(children1));

        children2.push_back(std::move(mulNode));
        children2.push_back(std::move(addNode));

        auto lessThan = ASTNode("", "<", std::move(children2));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildConditionalExpressionAST(queue);
        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build not conditional expression") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NOT, "!", 0),          Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),         Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};

        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        std::vector<std::unique_ptr<ASTNode>> children3 = {};

        children2.push_back(std::move(xNode));
        children2.push_back(std::move(yNode));

        auto lessThan = std::make_unique<ASTNode>("", "<", std::move(children2));

        children3.push_back(std::move(lessThan));

        auto notNode = ASTNode("", "!", std::move(children3));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildConditionalExpressionAST(queue);
        REQUIRE(notNode == *(result.get()));
    }

    SECTION("Build binary conditional expression") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),     Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0), Token(LEXICAL_TOKEN_TYPE::ANDAND, "&&", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::GREATER_THAN, ">", 0),  Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode1 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode1 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        std::vector<std::unique_ptr<ASTNode>> children3 = {};
        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto lessThan = std::make_unique<ASTNode>("", "<", std::move(children));
        children2.push_back(std::move(xNode1));
        children2.push_back(std::move(yNode1));

        auto moreThan = std::make_unique<ASTNode>("", ">", std::move(children2));
        children3.push_back(std::move(lessThan));
        children3.push_back(std::move(moreThan));
        auto andandNode = ASTNode("", "&&", std::move(children3));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildConditionalExpressionAST(queue);
        REQUIRE(andandNode == *(result.get()));
    }

    SECTION("While loop test statement") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "while", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::ANDAND, "&&", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::GREATER_THAN, ">", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 1),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 1),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };
        std::unique_ptr<ASTNode> xNode = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode1 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode1 = std::make_unique<ASTNode>("y", "var");
        std::unique_ptr<ASTNode> xNode2 = std::make_unique<ASTNode>("x", "var");
        std::unique_ptr<ASTNode> yNode2 = std::make_unique<ASTNode>("y", "var");

        std::vector<std::unique_ptr<ASTNode>> children = {};
        std::vector<std::unique_ptr<ASTNode>> children2 = {};
        std::vector<std::unique_ptr<ASTNode>> children3 = {};
        std::vector<std::unique_ptr<ASTNode>> children4 = {};
        std::vector<std::unique_ptr<ASTNode>> children5 = {};
        std::vector<std::unique_ptr<ASTNode>> children6 = {};

        children.push_back(std::move(xNode));
        children.push_back(std::move(yNode));

        auto lessThan = std::make_unique<ASTNode>("", "<", std::move(children));
        children2.push_back(std::move(xNode1));
        children2.push_back(std::move(yNode1));

        auto moreThan = std::make_unique<ASTNode>("", ">", std::move(children2));
        children3.push_back(std::move(lessThan));
        children3.push_back(std::move(moreThan));
        auto andandNode = std::make_unique<ASTNode>("", "&&", std::move(children3));

        children4.push_back(std::move(xNode2));
        children4.push_back(std::move(yNode2));

        auto assign = std::make_unique<ASTNode>("", "assign", std::move(children4));
        children6.push_back(std::move(assign));
        auto stmtList = std::make_unique<ASTNode>("", "stmtList", std::move(children6));

        children5.push_back(std::move(andandNode));
        children5.push_back(std::move(stmtList));
        auto expected = WhileNode(std::move(children5));

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildWhileAST(queue);
        REQUIRE(expected == *(result.get()));
    }
}
