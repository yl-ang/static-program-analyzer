#include <memory>

#include "catch.hpp"
#include "sp/ast/AstNode.h"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/de/DesignExtractor.h"

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
    09 a = 1;
    */

    // x = y;
    std::shared_ptr<VariableNode> xNode = std::make_shared<VariableNode>(VariableNode("x", 1));
    std::shared_ptr<VariableNode> yNode = std::make_shared<VariableNode>(VariableNode("y", 1));
    std::vector<std::shared_ptr<ASTNode>> childrenA = {};
    childrenA.push_back(xNode);
    childrenA.push_back(yNode);
    auto equalNode = std::make_shared<AssignmentNode>(childrenA, 1);

    // read a;
    std::shared_ptr<VariableNode> aNode = std::make_shared<VariableNode>(VariableNode("a", 2));
    std::vector<std::shared_ptr<ASTNode>> childrenR = {};
    childrenR.push_back(aNode);
    auto readNode = std::make_shared<ReadNode>(childrenR, 2);

    // print z;
    std::shared_ptr<VariableNode> zNode = std::make_shared<VariableNode>(VariableNode("z", 3));
    std::vector<std::shared_ptr<ASTNode>> childrenP = {};
    childrenP.push_back(zNode);
    auto printNode = std::make_shared<PrintNode>(3, childrenP);

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

    childrenW.push_back(cNode);
    childrenW.push_back(dNode);

    auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, childrenW, 4);
    childrenW2.push_back(eNode);
    childrenW2.push_back(fNode);

    auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, childrenW2, 4);
    childrenW3.push_back(WlessThan);
    childrenW3.push_back(WmoreThan);
    auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, childrenW3, 4);

    childrenW4.push_back(gNode);
    childrenW4.push_back(hNode);

    auto assign = std::make_shared<AssignmentNode>(childrenW4, 5);
    childrenW6.push_back(assign);
    auto WstmtList = std::make_shared<StatementListNode>(childrenW6);

    childrenW5.push_back(WandandNode);
    childrenW5.push_back(WstmtList);

    auto whileNode = std::make_shared<WhileNode>(childrenW5, 4);

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
    std::shared_ptr<VariableNode> xNode2 = std::make_shared<VariableNode>(VariableNode("x", 9));
    std::shared_ptr<ConstantNode> constNode = std::make_shared<ConstantNode>(ConstantNode("1", 9));

    std::vector<std::shared_ptr<ASTNode>> childrenI = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI2 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI3 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI4 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI5 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI6 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI7 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI8 = {};
    std::vector<std::shared_ptr<ASTNode>> childrenI9 = {};

    childrenI.push_back(iNode);
    childrenI.push_back(jNode);

    auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, childrenI, 6);
    childrenI2.push_back(kNode);
    childrenI2.push_back(lNode);

    auto ImoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, childrenI2, 6);
    childrenI3.push_back(IlessThan);
    childrenI3.push_back(ImoreThan);
    auto IandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, childrenI3, 6);

    childrenI4.push_back(iNode1);
    childrenI4.push_back(jNode1);

    auto Iassign = std::make_shared<AssignmentNode>(childrenI4, 7);
    childrenI5.push_back(Iassign);
    auto IstmtList = std::make_shared<StatementListNode>(childrenI5);

    childrenI6.push_back(aNode1);
    childrenI6.push_back(bNode1);
    auto Iassign1 = std::make_shared<AssignmentNode>(childrenI6, 8);
    childrenI7.push_back(Iassign1);
    auto IstmtList2 = std::make_shared<StatementListNode>(childrenI7);

    childrenI8.push_back(IandandNode);
    childrenI8.push_back(IstmtList);
    childrenI8.push_back(IstmtList2);

    auto ifNode = std::make_shared<IfNode>(IandandNode, IstmtList, IstmtList2, 6);

    childrenI9.push_back(xNode2);
    childrenI9.push_back(constNode);

    auto xAssign = std::make_shared<AssignmentNode>(childrenI9, 9);

    std::vector<std::shared_ptr<ASTNode>> childrenProc = {};
    std::vector<std::shared_ptr<ASTNode>> childrenProc1 = {};
    childrenProc.push_back(equalNode);
    childrenProc.push_back(readNode);
    childrenProc.push_back(printNode);
    childrenProc.push_back(whileNode);
    childrenProc.push_back(ifNode);
    childrenProc.push_back(xAssign);

    auto stmtListProc = std::make_shared<StatementListNode>(childrenProc);
    childrenProc1.push_back(stmtListProc);
    auto ProcNode = std::make_shared<ProcedureNode>("main", childrenProc1);
    std::vector<std::shared_ptr<ASTNode>> childrenProg = {};
    childrenProg.push_back(ProcNode);

    auto ProgNode = std::make_shared<ProgramNode>((childrenProg));

    /*
   procedure test {
   01 read a;
   }
    */
    auto variableA = std::make_shared<VariableNode>("a", 1);
    std::vector<std::shared_ptr<ASTNode>> children = {};
    children.push_back(variableA);
    auto readA = std::make_shared<ReadNode>(children, 1);
    std::vector<std::shared_ptr<ASTNode>> children2 = {};
    children2.push_back(readA);
    auto stmtListProc1 = std::make_shared<StatementListNode>(children2);
    std::vector<std::shared_ptr<ASTNode>> children3 = {};
    children3.push_back(stmtListProc1);
    auto ProcNode2 = std::make_shared<ProcedureNode>("test", children3);
    std::vector<std::shared_ptr<ASTNode>> childrenProg2 = {};

    childrenProg2.push_back(ProcNode2);
    auto ProgNode2 = std::make_shared<ProgramNode>((childrenProg2));

    std::vector<std::shared_ptr<ASTNode>> childrenProg3 = {};

    childrenProg3.push_back(ProcNode);
    childrenProg3.push_back(ProcNode2);

    auto ProgNode3 = std::make_shared<ProgramNode>((childrenProg3));

    SECTION("Variables extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::string> expectedVariables = {"x", "y", "a", "z", "c", "d", "e", "f",
                                                             "g", "h", "i", "j", "k", "l", "b"};
        REQUIRE(expectedVariables == designExtractor->getVariables());
    }

    SECTION("Single line procedure has variables extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::string> expectedVariables = {"a"};
        REQUIRE(expectedVariables == designExtractor->getVariables());
    }

    SECTION("Multiple procedures variables extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::string> expectedVariables = {"x", "y", "a", "z", "c", "d", "e", "f",
                                                             "g", "h", "i", "j", "k", "l", "b"};
        REQUIRE(expectedVariables == designExtractor->getVariables());
    }

    SECTION("Constants extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::string> expectedConstants = {"1"};
        REQUIRE(expectedConstants == designExtractor->getConstants());
    }

    SECTION("Single line procedure has constants extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::string> expectedConstants = {};
        REQUIRE(expectedConstants == designExtractor->getConstants());
    }

    SECTION("Multiple procedures variables extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::string> expectedConstants = {"1"};
        REQUIRE(expectedConstants == designExtractor->getConstants());
    }

    SECTION("Procedure extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::string> expectedProcedures = {"main"};
        REQUIRE(expectedProcedures == designExtractor->getProcedures());
    }

    SECTION("Multiple procedure extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::string> expectedProcedures = {"main", "test"};
        REQUIRE(expectedProcedures == designExtractor->getProcedures());
    }

    SECTION("Statements extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<Stmt> expectedStatements = {
            Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::READ, 2},   Stmt{StatementType::PRINT, 3},
            Stmt{StatementType::WHILE, 4},  Stmt{StatementType::ASSIGN, 5}, Stmt{StatementType::IF, 6},
            Stmt{StatementType::ASSIGN, 7}, Stmt{StatementType::ASSIGN, 8}, Stmt{StatementType::ASSIGN, 9}};
        REQUIRE(expectedStatements == designExtractor->getStatements());
    }

    SECTION("Single line procedure has statements extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<Stmt> expectedStatements = {Stmt{StatementType::READ, 1}};
        REQUIRE(expectedStatements == designExtractor->getStatements());
    }

    SECTION("Multiple procedure statements extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<Stmt> expectedStatements = {Stmt{StatementType::READ, 1},   Stmt{StatementType::ASSIGN, 1},
                                                       Stmt{StatementType::READ, 2},   Stmt{StatementType::PRINT, 3},
                                                       Stmt{StatementType::WHILE, 4},  Stmt{StatementType::ASSIGN, 5},
                                                       Stmt{StatementType::IF, 6},     Stmt{StatementType::ASSIGN, 7},
                                                       Stmt{StatementType::ASSIGN, 8}, Stmt{StatementType::ASSIGN, 9}};
        REQUIRE(expectedStatements == designExtractor->getStatements());
    }

    SECTION("Follows extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedFollows = {{1, 2}, {2, 3}, {3, 4}, {4, 6}, {6, 9}};
        REQUIRE(expectedFollows == designExtractor->getFollows());
    }

    SECTION("Single line procedure has follows extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedFollows = {};
        REQUIRE(expectedFollows == designExtractor->getFollows());
    }

    SECTION("Multiple procedures follows extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedFollows = {{1, 2}, {2, 3}, {3, 4}, {4, 6}, {6, 9}};
        REQUIRE(expectedFollows == designExtractor->getFollows());
    }

    SECTION("Parent extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedParent = {{4, 5}, {6, 7}, {6, 8}};
        REQUIRE(expectedParent == designExtractor->getParent());
    }

    SECTION("Single line procedure has parent extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedParent = {};
        REQUIRE(expectedParent == designExtractor->getParent());
    }

    SECTION("Multiple procedures parent extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
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

    SECTION("Single line procedure has uses extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {};
        REQUIRE(expectedUses == designExtractor->getUses());
    }

    SECTION("Multiple procedures uses extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {
            {1, "y"}, {3, "z"}, {4, "c"}, {4, "d"}, {4, "e"}, {4, "f"}, {4, "h"}, {5, "h"},
            {6, "i"}, {6, "j"}, {6, "k"}, {6, "l"}, {6, "j"}, {7, "j"}, {6, "b"}, {8, "b"}};
        REQUIRE(expectedUses == designExtractor->getUses());
    }

    SECTION("Modifies extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {
            {1, "x"}, {2, "a"}, {4, "g"}, {5, "g"}, {6, "i"}, {6, "a"}, {7, "i"}, {8, "a"}, {9, "x"}};
        REQUIRE(expectedModifies == designExtractor->getModifies());
    }

    SECTION("Single line procedure has modifies extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{1, "a"}};
        REQUIRE(expectedModifies == designExtractor->getModifies());
    }

    SECTION("Multiple procedures modifies extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {
            {1, "a"}, {1, "x"}, {2, "a"}, {4, "g"}, {5, "g"}, {6, "i"}, {6, "a"}, {7, "i"}, {8, "a"}, {9, "x"}};
        REQUIRE(expectedModifies == designExtractor->getModifies());
    }

    SECTION("Pattern extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {
            {1, {"x", "y"}}, {5, {"g", "h"}}, {7, {"i", "j"}}, {8, {"a", "b"}}, {9, {"x", "1"}}};
        REQUIRE(expectedPattern == designExtractor->getPattern());
    }
    SECTION("Single line procedure has pattern extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode2);
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {};
        REQUIRE(expectedPattern == designExtractor->getPattern());
    }

    SECTION("Multiple procedures pattern extracted correctly") {
        DesignExtractor *designExtractor = new DesignExtractor();
        designExtractor->extract(ProgNode3);
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {
            {1, {"x", "y"}}, {5, {"g", "h"}}, {7, {"i", "j"}}, {8, {"a", "b"}}, {9, {"x", "1"}}};
        REQUIRE(expectedPattern == designExtractor->getPattern());
    }
}
