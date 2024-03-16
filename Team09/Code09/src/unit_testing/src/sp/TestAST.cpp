
#include <memory>
#include <queue>

#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/de/AstVisitor.h"
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
        ConstantNode expectedNode = ConstantNode("1", 0);

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0)};
        VariableNode expectedNode = VariableNode("a", 0);
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build assignment to const ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        };
        auto queue = makeTokenQueue(inputTokenArray);

        std::shared_ptr<VariableNode> nameNode = std::make_shared<VariableNode>("a", 0);
        std::shared_ptr<ConstantNode> integerNode = std::make_shared<ConstantNode>("1", 0);

        auto expectedNode = AssignmentNode(nameNode, integerNode, 0);

        auto result = ast.buildAssignmentAST(queue);
        REQUIRE(expectedNode == *(result.get()));
    }

    SECTION("Build assignment to var ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0), Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0), Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildAssignmentAST(queue);  // Do this first

        std::shared_ptr<VariableNode> nameNode = std::make_shared<VariableNode>("a", 0);
        std::shared_ptr<VariableNode> integerNode = std::make_shared<VariableNode>("b", 0);

        AssignmentNode expectedNode = AssignmentNode(nameNode, integerNode, 0);

        REQUIRE(expectedNode == *(result.get()));  // Now this should work
    }

    SECTION("Build adding two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
                                              Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "c", 0)};
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::shared_ptr<VariableNode> constNode = std::make_shared<VariableNode>("b", 0);
        std::shared_ptr<VariableNode> const2Node = std::make_shared<VariableNode>("c", 0);

        auto addNode = ExpressionNode(ADD, constNode, const2Node, 0);

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build adding multiple terms correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode, yNode, 0);

        auto addNode2 = ExpressionNode(ADD, addNode, zNode, 0);

        REQUIRE(addNode2 == *(result.get()));
    }

    SECTION("Build multiplying two terms correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
                                              Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildTermAST(queue);  // Do this first
        std::shared_ptr<VariableNode> xtNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);

        auto mulNode = ExpressionNode(MUL, xtNode, yNode, 0);

        REQUIRE(mulNode == *(result.get()));
    }

    SECTION("Build add then mul correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, yNode, zNode, 0);

        auto addNode = ExpressionNode(ADD, xNode, mulNode, 0);

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build mul then add correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildExpressionAST(queue);  // Do this first
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto addNode = ExpressionNode(ADD, mulNode, zNode, 0);

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
        auto xNode = std::make_shared<VariableNode>("x", 1);
        auto yNode = std::make_shared<VariableNode>("y", 1);

        std::vector<std::shared_ptr<StatementNode>> children1 = {};

        auto equalNode = std::make_shared<AssignmentNode>(xNode, yNode, 1);

        children1.push_back(equalNode);
        auto stmtList = std::make_shared<StatementListNode>(children1);

        auto procedure = ProcedureNode("a", stmtList);

        REQUIRE(procedure == *(result.get()));
    }

    SECTION("Build read statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::READ, "read", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildReadAST(queue);

        std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>("a", 0);

        auto expectedNode = ReadNode(aNode, 0);

        REQUIRE(expectedNode == *(result.get()));
    }
    SECTION("Build read procedure correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::READ, "read", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 0),
        };

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildProcedureAST(queue);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);

        std::vector<std::shared_ptr<StatementNode>> children1 = {};

        auto readNode = std::make_shared<ReadNode>(yNode, 0);

        children1.push_back(readNode);
        auto stmtList = std::make_shared<StatementListNode>(children1);

        auto procedure = ProcedureNode("a", stmtList);

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

        std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>("a", 0);

        auto expectedNode = PrintNode(aNode, 0);

        REQUIRE(expectedNode == *(result.get()));
    }
    SECTION("Build print procedure correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::PROCEDURE, "procedure", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::PRINT, "print", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildProcedureAST(queue);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);

        std::vector<std::shared_ptr<StatementNode>> children1 = {};

        auto printNode = std::make_shared<PrintNode>(yNode, 0);

        children1.push_back(printNode);
        auto stmtList = std::make_shared<StatementListNode>(children1);

        auto procedure = ProcedureNode("a", stmtList);

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
        std::shared_ptr<VariableNode> constNode = std::make_shared<VariableNode>("b", 0);
        std::shared_ptr<VariableNode> const2Node = std::make_shared<VariableNode>("c", 0);

        auto addNode = ExpressionNode(ADD, constNode, const2Node, 0);

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
        std::shared_ptr<VariableNode> constNode = std::make_shared<VariableNode>("b", 0);
        std::shared_ptr<VariableNode> const2Node = std::make_shared<VariableNode>("c", 0);

        auto addNode = ExpressionNode(ADD, constNode, const2Node, 0);

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
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        std::vector<std::shared_ptr<ExpressionNode>> children = {};
        std::vector<std::shared_ptr<ExpressionNode>> children1 = {};

        children.push_back(xNode);
        children.push_back(yNode);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);
        children1.push_back(mulNode);

        children1.push_back(zNode);

        auto addNode = ExpressionNode(ADD, mulNode, zNode, 0);

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
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto addNode = ExpressionNode(ADD, mulNode, zNode, 0);

        REQUIRE(addNode == *(result.get()));
    }

    SECTION("Build parenthesised const factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);
        ConstantNode expectedNode = ConstantNode("1", 0);

        REQUIRE(expectedNode == *(result.get()));
    }
    SECTION("Build parenthesised variable factor ast correctly") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
                                              Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0)};
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildFactorAST(queue);

        VariableNode expectedNode = VariableNode("a", 0);

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
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        std::vector<std::shared_ptr<ExpressionNode>> children1 = {};

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode, yNode, 0);

        auto mulNode = ExpressionNode(MUL, addNode, zNode, 0);

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
        std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>("a", 0);
        std::shared_ptr<VariableNode> bNode = std::make_shared<VariableNode>("b", 0);
        std::shared_ptr<VariableNode> cNode = std::make_shared<VariableNode>("c", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, bNode, cNode, 0);

        auto mulNode = ExpressionNode(MUL, aNode, addNode, 0);

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
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);
        std::shared_ptr<VariableNode> zNode2 = std::make_shared<VariableNode>("z", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode, yNode, 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, zNode, addNode, 0);

        auto addNode2 = ExpressionNode(ADD, mulNode, zNode2, 0);

        REQUIRE(addNode2 == *(result.get()));
    }

    SECTION("Build rel_factors correctly for var") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);
        VariableNode aNode = VariableNode("a", 0);

        REQUIRE(aNode == *(result.get()));
    }

    SECTION("Build rel_factors correctly for const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);
        ConstantNode oneNode = ConstantNode("1", 0);

        REQUIRE(oneNode == *(result.get()));
    }
    SECTION("Build rel_factors correctly for expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0), Token(LEXICAL_TOKEN_TYPE::MUL, "*", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildRelationalFactorAST(queue);  // Do this first
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, yNode, zNode, 0);

        auto addNode = ExpressionNode(ADD, xNode, mulNode, 0);

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
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);
        auto oneNode = std::make_shared<ConstantNode>("1", 0);

        auto lessThan = ExpressionNode(LESS_THAN, zNode, oneNode, 0);

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
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::vector<std::shared_ptr<ExpressionNode>> children = {};

        auto lessThan = ExpressionNode(LESS_THAN, zNode, yNode, 0);

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with const and var") {
        std::vector<Token> inputTokenArray = {Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
                                              Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
                                              Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0)};

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        auto zNode = std::make_shared<VariableNode>("z", 0);
        auto oneNode = std::make_shared<ConstantNode>("1", 0);

        auto lessThan = ExpressionNode(LESS_THAN, oneNode, zNode, 0);

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with const and const") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 0),
            Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::INTEGER, "2", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildRelationalExpressionAST(queue);
        auto twoNode = std::make_shared<ConstantNode>("2", 0);
        auto oneNode = std::make_shared<ConstantNode>("1", 0);

        auto lessThan = ExpressionNode(LESS_THAN, oneNode, twoNode, 0);

        REQUIRE(lessThan == *(result.get()));
    }

    SECTION("Build relational expression with var and expr") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::NAME, "z", 0), Token(LEXICAL_TOKEN_TYPE::LESS_THAN, "<", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0), Token(LEXICAL_TOKEN_TYPE::ADD, "+", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
        };

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>("z", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode, yNode, 0);

        auto lessThan = ExpressionNode(LESS_THAN, zNode, addNode, 0);
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

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode2, yNode2, 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto lessThan = ExpressionNode(LESS_THAN, mulNode, addNode, 0);

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

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode2, yNode2, 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto lessThan = ExpressionNode(LESS_THAN, mulNode, addNode, 0);

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

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode2, yNode2, 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto lessThan = ExpressionNode(LESS_THAN, mulNode, addNode, 0);

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

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);

        auto addNode = std::make_shared<ExpressionNode>(ADD, xNode2, yNode2, 0);

        auto mulNode = std::make_shared<ExpressionNode>(MUL, xNode, yNode, 0);

        auto lessThan = ExpressionNode(LESS_THAN, mulNode, addNode, 0);

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

        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);

        auto lessThan = std::make_shared<ExpressionNode>(LESS_THAN, xNode, yNode, 0);

        auto notNode = ExpressionNode(NOT, lessThan, 0);

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
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode1 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode1 = std::make_shared<VariableNode>("y", 0);

        auto lessThan = std::make_shared<ExpressionNode>(LESS_THAN, xNode, yNode, 0);

        auto moreThan = std::make_shared<ExpressionNode>(GREATER_THAN, xNode1, yNode1, 0);

        auto andandNode = ExpressionNode(ANDAND, lessThan, moreThan, 0);

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildConditionalExpressionAST(queue);
        REQUIRE(andandNode == *(result.get()));
    }

    SECTION("While loop test statement") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::WHILE, "while", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NOT, "!", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),

            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL_CHECK, "==", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::ANDAND, "&&", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, "(", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK, "!=", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),

            Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, ")", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode1 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode1 = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);

        auto lessThan = std::make_shared<ExpressionNode>(EQUAL_CHECK, xNode, yNode, 0);

        auto moreThan = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, xNode1, yNode1, 0);

        auto andandNode = std::make_shared<ExpressionNode>(ANDAND, lessThan, moreThan, 0);

        auto notNode = std::make_shared<ExpressionNode>(NOT, andandNode, 0);

        auto assign = std::make_shared<AssignmentNode>(xNode2, yNode2, 0);

        auto stmtList = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({assign}));

        auto expected = WhileNode(notNode, stmtList, 0);

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildWhileAST(queue);
        REQUIRE(expected == *(result.get()));
    }

    SECTION("Build if-then-else statement") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::IF, "if", 0),
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
            Token(LEXICAL_TOKEN_TYPE::THEN, "then", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "x", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "y", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
            Token(LEXICAL_TOKEN_TYPE::ELSE, "else", 0),
            Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
            Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        };
        std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode1 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode1 = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>("x", 0);
        std::shared_ptr<VariableNode> yNode2 = std::make_shared<VariableNode>("y", 0);
        std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>("a", 0);
        std::shared_ptr<VariableNode> bNode = std::make_shared<VariableNode>("b", 0);

        std::vector<std::shared_ptr<StatementNode>> children5 = {};
        std::vector<std::shared_ptr<StatementNode>> children7 = {};

        auto lessThan = std::make_shared<ExpressionNode>(LESS_THAN, xNode, yNode, 0);

        auto moreThan = std::make_shared<ExpressionNode>(GREATER_THAN, xNode1, yNode1, 0);
        auto andandNode = std::make_shared<ExpressionNode>(ANDAND, lessThan, moreThan, 0);

        auto assign = std::make_shared<AssignmentNode>(xNode2, yNode2, 0);
        children5.push_back(assign);
        auto stmtList = std::make_shared<StatementListNode>(children5);

        auto assign1 = std::make_shared<AssignmentNode>(aNode, bNode, 0);
        children7.push_back(assign1);
        auto stmtList2 = std::make_shared<StatementListNode>(children7);

        auto expected = IfNode(andandNode, stmtList, stmtList2, 0);

        auto queue = makeTokenQueue(inputTokenArray);

        auto result = ast.buildIfAST(queue);
        REQUIRE(expected == *(result.get()));
    }

    SECTION("Build call statement ast correctly") {
        std::vector<Token> inputTokenArray = {
            Token(LEXICAL_TOKEN_TYPE::CALL, "call", 0),
            Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
            Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 0),
        };
        auto queue = makeTokenQueue(inputTokenArray);
        auto result = ast.buildCallAST(queue);

        auto expectedNode = CallNode("a", 0);

        REQUIRE(expectedNode == *(result.get()));
    }
}
