#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/ast/AstNode.h"
#include "sp/de/DesignExtractor.h"
#include "sp/de/EntityExtractor.h"
#include "sp/de/FollowsExtractor.h"
#include "sp/de/ModifiesExtractor.h"
#include "sp/de/ParentExtractor.h"
#include "sp/de/PatternExtractor.h"
#include "sp/de/UsesExtractor.h"
#include "sp/tokenizer/SpTokenizer.h"

TEST_CASE("Design Extractor Test using Build AST") {
    SpTokenizer tokenizer;
    AST ast;
    DesignExtractor designExtractor;
    std::vector<std::string> program = {"procedure", "main", "{", "x", "=", "y", ";", "}"};

    for (std::string str : program) {
        std::cout << str << std::endl;
    }
    std::vector<Token> tokens = tokenizer.tokenize(program);
    std::shared_ptr<ProgramNode> astRoot = ast.buildAST(tokens);
    designExtractor.extract(std::move(astRoot));

    std::unordered_set<std::string> expectedVariables = {"x", "y"};
    std::cout << "Var Size: " << designExtractor.getVariables().size() << std::endl;
    REQUIRE(expectedVariables == designExtractor.getVariables());
}

TEST_CASE("Simple Design Extractor Test") {
    // x = y;
    std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>(VariableNode("x", 1));
    std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>(VariableNode("y", 1));
    std::vector<std::shared_ptr<ASTNode>> childrenA = {};
    childrenA.push_back(std::move(xNode));
    childrenA.push_back(std::move(yNode));
    auto equalNode = std::make_shared<AssignmentNode>(std::move(childrenA), 1);

    std::vector<std::shared_ptr<ASTNode>> childrenProc = {};
    std::vector<std::shared_ptr<ASTNode>> childrenProc1 = {};
    childrenProc.push_back(std::move(equalNode));

    auto stmtListProc = std::make_shared<StatementListNode>(std::move(childrenProc));
    childrenProc1.push_back(std::move(stmtListProc));
    auto ProcNode = std::make_shared<ProcedureNode>("main", std::move(childrenProc1));

    std::vector<std::shared_ptr<ASTNode>> childrenProg = {};
    childrenProg.push_back(std::move(ProcNode));

    auto ProgNode = std::make_shared<ProgramNode>((childrenProg));

    DesignExtractor designExtractor;
    designExtractor.extract(ProgNode);
    std::unordered_set<std::string> expectedVariables = {"x", "y"};
    REQUIRE(expectedVariables == designExtractor.getVariables());
}

TEST_CASE("Design Extractor Tests") {
    /*
    01  x = y;
    02  read a;
    03  print z;
    04  while ((c < d) && (e > f)) {
    05      g = h;
        }
    06  if (( i < j ) && ( k > l )) then {
    07      i = j;
        } else {
    08      a = b;
        }
    */

    // x = y;
    std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>(VariableNode("x", 1));
    std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>(VariableNode("y", 1));
    std::vector<std::shared_ptr<ASTNode>> childrenA = {};
    childrenA.push_back(std::move(xNode));
    childrenA.push_back(std::move(yNode));
    auto equalNode = std::make_shared<AssignmentNode>(std::move(childrenA), 1);

    // read a;
    std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>(VariableNode("a", 2));
    std::vector<std::shared_ptr<ASTNode>> childrenR = {};
    childrenR.push_back(std::move(aNode));
    auto readNode = std::make_shared<ReadNode>(std::move(childrenR), 2);

    // print z;
    std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>(VariableNode("z", 3));
    std::vector<std::shared_ptr<ASTNode>> childrenP = {};
    childrenP.push_back(std::move(zNode));
    auto printNode = std::make_shared<PrintNode>(3, std::move(childrenP));

    // while ((c < d) && (e > f)) {
    //     g = h;
    // }
    std::shared_ptr<VariableNode> cNode = std::make_shared<VariableNode>(VariableNode("c", 4));
    std::shared_ptr<VariableNode> dNode = std::make_shared<VariableNode>(VariableNode("d", 4));
    std::shared_ptr<VariableNode> eNode = std::make_shared<VariableNode>(VariableNode("e", 4));
    std::shared_ptr<VariableNode> fNode = std::make_shared<VariableNode>(VariableNode("f", 4));
    std::shared_ptr<VariableNode> gNode = std::make_shared<VariableNode>(VariableNode("g", 5));
    std::shared_ptr<VariableNode> hNode = std::make_shared<VariableNode>(VariableNode("h", 5));

    std::vector<std::shared_ptr<ASTNode>> childrenW = {};
    std::vector<std::shared_ptr<ASTNode>> childrenW2 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenW3 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenW4 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenW5 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenW6 = {};

    childrenW.push_back(std::move(cNode));
    childrenW.push_back(std::move(dNode));

    auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, std::move(childrenW), 4);
    childrenW2.push_back(std::move(eNode));
    childrenW2.push_back(std::move(fNode));

    auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, std::move(childrenW2), 4);
    childrenW3.push_back(std::move(WlessThan));
    childrenW3.push_back(std::move(WmoreThan));
    auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, std::move(childrenW3), 4);

    childrenW4.push_back(std::move(gNode));
    childrenW4.push_back(std::move(hNode));

    auto assign = std::make_shared<AssignmentNode>(std::move(childrenW4), 5);
    childrenW6.push_back(std::move(assign));
    auto WstmtList = std::make_shared<StatementListNode>(std::move(childrenW6));

    childrenW5.push_back(std::move(WandandNode));
    childrenW5.push_back(std::move(WstmtList));

    auto whileNode = std::make_shared<WhileNode>(std::move(childrenW5), 4);

    // if (( i < j ) && ( k > l )) then {
    //       i = j;
    // } else {
    //       a = b;
    // }

    std::shared_ptr<VariableNode> iNode = std::make_shared<VariableNode>(VariableNode("i", 6));
    std::shared_ptr<VariableNode> jNode = std::make_shared<VariableNode>(VariableNode("j", 6));
    std::shared_ptr<VariableNode> kNode = std::make_shared<VariableNode>(VariableNode("k", 6));
    std::shared_ptr<VariableNode> lNode = std::make_shared<VariableNode>(VariableNode("l", 6));
    std::shared_ptr<VariableNode> iNode1 = std::make_shared<VariableNode>(VariableNode("i", 7));
    std::shared_ptr<VariableNode> jNode1 = std::make_shared<VariableNode>(VariableNode("j", 7));
    std::shared_ptr<VariableNode> aNode1 = std::make_shared<VariableNode>(VariableNode("a", 8));
    std::shared_ptr<VariableNode> bNode1 = std::make_shared<VariableNode>(VariableNode("b", 8));

    std::vector<std::shared_ptr<ASTNode>> childrenI = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI2 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI3 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI4 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI5 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI6 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI7 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI8 = {};

    childrenI.push_back(std::move(iNode));
    childrenI.push_back(std::move(jNode));

    auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, std::move(childrenI), 6);
    childrenI2.push_back(std::move(kNode));
    childrenI2.push_back(std::move(lNode));

    auto ImoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, std::move(childrenI2), 6);
    childrenI3.push_back(std::move(IlessThan));
    childrenI3.push_back(std::move(ImoreThan));
    auto IandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, std::move(childrenI3), 6);

    childrenI4.push_back(std::move(iNode1));
    childrenI4.push_back(std::move(jNode1));

    auto Iassign = std::make_shared<AssignmentNode>(std::move(childrenI4), 7);
    childrenI5.push_back(std::move(Iassign));
    auto IstmtList = std::make_shared<StatementListNode>(std::move(childrenI5));

    childrenI6.push_back(std::move(aNode1));
    childrenI6.push_back(std::move(bNode1));
    auto Iassign1 = std::make_shared<AssignmentNode>(std::move(childrenI6), 8);
    childrenI7.push_back(std::move(Iassign1));
    auto IstmtList2 = std::make_shared<StatementListNode>(std::move(childrenI7));

    childrenI8.push_back(std::move(IandandNode));
    childrenI8.push_back(std::move(IstmtList));
    childrenI8.push_back(std::move(IstmtList2));

    auto ifNode = std::make_shared<IfNode>(std::move(childrenI8), 6);

    std::vector<std::shared_ptr<ASTNode>> childrenProc = {};
    std::vector<std::shared_ptr<ASTNode>> childrenProc1 = {};
    childrenProc.push_back(std::move(equalNode));
    childrenProc.push_back(std::move(readNode));
    childrenProc.push_back(std::move(printNode));
    childrenProc.push_back(std::move(whileNode));
    childrenProc.push_back(std::move(ifNode));

    auto stmtListProc = std::make_shared<StatementListNode>(std::move(childrenProc));
    childrenProc1.push_back(std::move(stmtListProc));
    auto ProcNode = std::make_shared<ProcedureNode>("main", std::move(childrenProc1));

    std::vector<std::shared_ptr<ASTNode>> childrenProg = {};
    childrenProg.push_back(std::move(ProcNode));

    auto ProgNode = std::make_shared<ProgramNode>((childrenProg));

    // designExtractor->extract(std::move(ProgNode));

    /*
            procedure main {
        01      x = y;
        02      read a;
        03      print z;
        04      while ((c < d) && (e > f)) {
        05          g = h;
                }
        06      if (( i < j ) && ( k > l )) then {
        07          i = j;
                } else {
        08          a = b;
                }
            }
    */

    SECTION("Variables extracted correctly") {
        DesignExtractor designExtractor;
        designExtractor.extract(ProgNode);
        std::unordered_set<std::string> expectedVariables = {"x", "y", "a", "z", "c", "d", "e", "f",
                                                             "g", "h", "i", "j", "k", "l", "b"};
        REQUIRE(expectedVariables == designExtractor.getVariables());
    }

    SECTION("Constants extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::string> expectedConstants = {};
        REQUIRE(expectedConstants == designExtractor->getConstants());
    }

    SECTION("Procedure extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::string> expectedProcedures = {"main"};
        REQUIRE(expectedProcedures == designExtractor->getProcedures());
    }

    SECTION("Statements extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<Stmt> expectedStatements = {Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::READ, 2},
                                                       Stmt{StatementType::PRINT, 3},  Stmt{StatementType::WHILE, 4},
                                                       Stmt{StatementType::ASSIGN, 5}, Stmt{StatementType::IF, 6},
                                                       Stmt{StatementType::ASSIGN, 7}, Stmt{StatementType::ASSIGN, 8}};
        REQUIRE(expectedStatements == designExtractor->getStatements());
    }

    SECTION("Follows extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedFollows = {{1, 2}, {2, 3}, {3, 4}, {4, 6}};
        REQUIRE(expectedFollows == designExtractor->getFollows());
    }

    SECTION("Parent extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedParent = {{4, 5}, {6, 7}, {6, 8}};
        REQUIRE(expectedParent == designExtractor->getParent());
    }

    SECTION("Uses extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {
            {1, "y"}, {3, "z"}, {4, "c"}, {4, "d"}, {4, "e"}, {4, "f"}, {4, "h"}, {5, "h"},
            {6, "i"}, {6, "j"}, {6, "k"}, {6, "l"}, {6, "j"}, {7, "j"}, {6, "b"}, {8, "b"}};
        REQUIRE(expectedUses == designExtractor->getUses());
    }

    SECTION("Modifies extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{1, "x"}, {2, "a"}, {4, "g"}, {5, "g"},
                                                                             {6, "i"}, {6, "a"}, {7, "i"}, {8, "a"}};
        REQUIRE(expectedModifies == designExtractor->getModifies());
    }

    SECTION("Pattern extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {
            {1, {"x", "y"}}, {5, {"g", "h"}}, {7, {"i", "j"}}, {8, {"a", "b"}}};
        REQUIRE(expectedPattern == designExtractor->getPattern());
    }
}
