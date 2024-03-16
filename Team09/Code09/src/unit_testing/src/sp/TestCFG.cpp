
#include <memory>
#include <vector>

#include "catch.hpp"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/VariableNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/IfNode.h"
#include "sp/ast/grammar_nodes/statements/PrintNode.h"
#include "sp/ast/grammar_nodes/statements/ReadNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/ast/grammar_nodes/statements/WhileNode.h"
#include "sp/cfg/Cfg.h"
#include "sp/de/AstVisitor.h"
#include "sp/tokenizer/Token.h"

using namespace std;  // NOLINT

TEST_CASE("CFG Build Tests") {
    CFG cfg;

    SECTION("Build if CFG correctly") {
        auto iNode = std::make_shared<VariableNode>("i", 1);
        auto constNode = std::make_shared<ConstantNode>("1", 1);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode, constNode, 1);

        auto xNode = std::make_shared<VariableNode>("x", 2);
        auto twoNode = std::make_shared<ConstantNode>("2", 2);

        auto ifConditionStatement = std::make_shared<AssignmentNode>(xNode, twoNode, 2);
        auto ifConditionStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifConditionStatement}));

        auto zNode = std::make_shared<VariableNode>("z", 3);
        auto threeNode = std::make_shared<ConstantNode>("3", 3);

        auto thenStatements = std::make_shared<AssignmentNode>(zNode, threeNode, 3);
        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({thenStatements}));

        auto ifNode = std::make_shared<IfNode>(ifCondition, ifConditionStmtList, thenStmtList, 1);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifNode}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {{1, {2, 3}}};

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while CFG correctly") {
        auto iNode = std::make_shared<VariableNode>("i", 1);
        auto constNode = std::make_shared<ConstantNode>("1", 1);

        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, iNode, constNode, 1);
        auto xNode = std::make_shared<VariableNode>("x", 2);
        auto yNode = std::make_shared<VariableNode>("y", 2);

        auto whileStatement1 = std::make_shared<AssignmentNode>(xNode, yNode, 2);
        auto iNode2 = std::make_shared<VariableNode>("i", 3);

        auto whileStatement2 = std::make_shared<ReadNode>(iNode2, 3);
        auto iNode3 = std::make_shared<VariableNode>("i", 4);
        auto constNode1 = std::make_shared<ConstantNode>("1", 4);

        auto whileStatement3 = std::make_shared<AssignmentNode>(iNode3, constNode1, 4);

        auto whileStmtList = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({whileStatement1, whileStatement2, whileStatement3}));
        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 1);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileNode}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);
        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {{1, {2}}, {2, {3}}, {3, {4}}, {4, {1}}};

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while then if CFG correctly") {
        /*
        01 x = 1;
        02 i = 2;
        03 while (i != 3) {
        04      x = y;
        05      read i;
        06      i = 6;
        }
        07 if (i == 7) then {
        08      x = 8;
            } else {
        09      z = 9;
            }
        10  z = 10;
        11  y = 11;
        12  x = 12;

        */
        auto xNode = std::make_shared<VariableNode>("x", 1);
        auto constNode = std::make_shared<ConstantNode>("0", 1);
        auto xAssignToZero = std::make_shared<AssignmentNode>(xNode, constNode, 1);

        auto iNode = std::make_shared<VariableNode>("i", 2);
        auto constNode1 = std::make_shared<ConstantNode>("5", 2);

        auto iAssignTo5 = std::make_shared<AssignmentNode>(iNode, constNode1, 2);

        auto iNode2 = std::make_shared<VariableNode>("i", 3);
        auto constNode2 = std::make_shared<ConstantNode>("5", 3);
        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, iNode2, constNode2, 3);

        auto xNode1 = std::make_shared<VariableNode>("x", 4);
        auto yNode1 = std::make_shared<VariableNode>("y", 4);
        auto whileStatement1 = std::make_shared<AssignmentNode>(xNode1, yNode1, 4);

        auto iNode3 = std::make_shared<VariableNode>("i", 5);

        auto whileStatement2 = std::make_shared<ReadNode>(iNode3, 5);

        auto iNode4 = std::make_shared<VariableNode>("i", 6);
        auto constNode3 = std::make_shared<ConstantNode>("5", 6);

        auto whileStatement3 = std::make_shared<AssignmentNode>(iNode4, constNode3, 6);

        auto iNode5 = std::make_shared<VariableNode>("i", 7);
        auto constNode4 = std::make_shared<ConstantNode>("5", 7);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode5, constNode4, 7);

        auto xNode2 = std::make_shared<VariableNode>("x", 8);
        auto constNode5 = std::make_shared<ConstantNode>("5", 8);
        auto thenStmt = std::make_shared<AssignmentNode>(xNode2, constNode5, 8);

        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({thenStmt}));

        auto zNode2 = std::make_shared<VariableNode>("z", 9);
        auto constNode6 = std::make_shared<ConstantNode>("5", 9);
        auto elseStmt = std::make_shared<AssignmentNode>(zNode2, constNode6, 9);

        auto elseStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({elseStmt}));

        auto zNode3 = std::make_shared<VariableNode>("z", 10);
        auto constNode7 = std::make_shared<ConstantNode>("5", 10);
        auto zEquals3 = std::make_shared<AssignmentNode>(zNode3, constNode7, 10);

        auto yNode3 = std::make_shared<VariableNode>("y", 11);
        auto constNode8 = std::make_shared<ConstantNode>("5", 11);
        auto yEquals4 = std::make_shared<AssignmentNode>(yNode3, constNode8, 11);

        auto yNode4 = std::make_shared<VariableNode>("y", 12);
        auto constNode9 = std::make_shared<ConstantNode>("5", 12);
        auto xEquals5 = std::make_shared<AssignmentNode>(yNode4, constNode9, 12);

        auto ifNode = std::make_shared<IfNode>(ifCondition, thenStmtList, elseStmtList, 7);

        auto whileStmtList = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({whileStatement1, whileStatement2, whileStatement3}));

        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 3);

        auto procStatementList = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>(
            {xAssignToZero, iAssignTo5, whileNode, ifNode, zEquals3, yEquals4, xEquals5}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {{1, {2}},  {2, {3}},   {3, {4, 7}}, {4, {5}},
                                                              {5, {6}},  {6, {3}},   {7, {8, 9}}, {8, {10}},
                                                              {9, {10}}, {10, {11}}, {11, {12}}};

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while in if CFG correctly") {
        /*
        01 if (i == 1) then {
        02    while (z != 2) {
        03        read i;
                }
            } else {
        04    read y;
            }
        */
        auto iNode = std::make_shared<VariableNode>("i", 1);
        auto constNode = std::make_shared<ConstantNode>("5", 1);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode, constNode, 1);

        auto zNode = std::make_shared<VariableNode>("z", 2);
        auto constNode1 = std::make_shared<ConstantNode>("5", 2);
        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode, constNode1, 2);

        auto iNode2 = std::make_shared<VariableNode>("i", 3);
        auto whileStatement1 = std::make_shared<ReadNode>(iNode2, 3);

        auto whileStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileStatement1}));

        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 2);

        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileNode}));

        auto yNode = std::make_shared<VariableNode>("y", 4);

        auto elseStatement = std::make_shared<ReadNode>(yNode, 4);

        auto elseStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({elseStatement}));

        auto ifNode = std::make_shared<IfNode>(ifCondition, thenStmtList, elseStmtList, 1);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifNode}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);
        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 4}},
            {2, {3}},
            {3, {2}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION("Build if in while CFG correctly") {
        /*
        01    while (z != 2) {
        02        if (i == 0) then {
        03            read i;
                } else {
        04            read a;
                }
        */
        auto zNode = std::make_shared<VariableNode>("z", 1);
        auto constNode = std::make_shared<ConstantNode>("5", 1);
        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode, constNode, 1);

        auto iNode = std::make_shared<VariableNode>("z", 2);
        auto constNode1 = std::make_shared<ConstantNode>("5", 2);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode, constNode1, 2);

        auto iNode2 = std::make_shared<VariableNode>("z", 3);

        auto readStmt = std::make_shared<ReadNode>(iNode2, 3);

        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readStmt}));

        auto iNode3 = std::make_shared<VariableNode>("z", 4);

        auto readStmt2 = std::make_shared<ReadNode>(iNode3, 4);

        auto elseStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readStmt2}));

        auto ifNode = std::make_shared<IfNode>(ifCondition, thenStmtList, elseStmtList, 2);
        auto whileStmtList = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifNode}));

        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 1);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileNode}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);
        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2}},
            {2, {3, 4}},
            {3, {1}},
            {4, {1}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION("Build if then while CFG correctly") {
        /*

        01 if (i == 7) then {
        02      read y;
            } else {
        03      read z;
            }
        04 while (i != 3) {
        05      read x;
        }
        */
        auto zNode = std::make_shared<VariableNode>("z", 1);
        auto constNode = std::make_shared<ConstantNode>("5", 1);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, zNode, constNode, 1);

        auto yNode = std::make_shared<VariableNode>("z", 2);
        auto readStmt = std::make_shared<ReadNode>(yNode, 2);

        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readStmt}));

        auto yNode2 = std::make_shared<VariableNode>("z", 3);
        auto readStmt2 = std::make_shared<ReadNode>(yNode2, 3);

        auto elseStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readStmt2}));

        auto ifNode = std::make_shared<IfNode>(ifCondition, thenStmtList, elseStmtList, 1);

        auto zNode2 = std::make_shared<VariableNode>("z", 4);
        auto constNode2 = std::make_shared<ConstantNode>("5", 4);
        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode2, constNode2, 4);

        auto zNode3 = std::make_shared<VariableNode>("z", 5);
        auto readStmt3 = std::make_shared<ReadNode>(zNode3, 5);

        auto whileStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readStmt3}));

        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 4);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifNode, whileNode}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}}, {2, {4}}, {3, {4}}, {4, {5}}, {5, {4}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while in if then something CFG correctly") {
        /*
        01 if (i == 1) then {
        02    while (z != 2) {
        03        read i;
                }
            } else {
        04    read y;
            }
        05  read x;
        */
        auto zNode = std::make_shared<VariableNode>("z", 1);
        auto constNode = std::make_shared<ConstantNode>("5", 1);
        auto ifCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, zNode, constNode, 1);

        auto zNode1 = std::make_shared<VariableNode>("z", 2);
        auto constNode1 = std::make_shared<ConstantNode>("5", 2);
        auto whileLoopCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode1, constNode1, 2);

        auto zNode2 = std::make_shared<VariableNode>("z", 3);
        auto whileStatement1 = std::make_shared<ReadNode>(zNode2, 3);

        auto whileStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileStatement1}));

        auto whileNode = std::make_shared<WhileNode>(whileLoopCondition, whileStmtList, 2);

        auto thenStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileNode}));
        auto zNode3 = std::make_shared<VariableNode>("z", 4);

        auto elseStatement = std::make_shared<ReadNode>(zNode3, 4);

        auto elseStmtList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({elseStatement}));

        auto ifNode = std::make_shared<IfNode>(ifCondition, thenStmtList, elseStmtList, 1);
        auto zNode4 = std::make_shared<VariableNode>("z", 5);

        auto readStatement = std::make_shared<ReadNode>(zNode4, 5);

        auto procStatementList =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifNode, readStatement}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStatementList);
        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 4}},
            {2, {3, 5}},
            {3, {2}},
            {4, {5}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION("Build while in else then something CFG correctly") {
        /*
        01 if (i == 1) then {
        02      read y;
            } else {
        03    while (z != 2) {
        04        read i;
                }
            }
        05  read x;
        */

        // ASTNode for if condition (i == 1)
        auto iNode = std::make_shared<VariableNode>("i", 1);
        auto constNode1 = std::make_shared<ConstantNode>("1", 1);
        auto ifCondition1 = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode, constNode1, 1);

        // ASTNode for then block (read y)
        auto yNode = std::make_shared<VariableNode>("y", 2);
        auto readYNode = std::make_shared<ReadNode>(yNode, 2);
        auto thenBlock = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readYNode}));

        // ASTNode for else block (while loop)
        auto zNode = std::make_shared<VariableNode>("z", 3);
        auto constNode2 = std::make_shared<ConstantNode>("2", 3);
        auto whileCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode, constNode2, 3);

        // ASTNode for read i inside while loop
        auto iNode2 = std::make_shared<VariableNode>("i", 4);
        auto readINode = std::make_shared<ReadNode>(iNode2, 4);
        auto whileBlock = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readINode}));

        auto whileNode = std::make_shared<WhileNode>(whileCondition, whileBlock, 3);

        // Assemble the ASTNodes into the CFG
        auto elseBranch = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileNode}));

        auto ifThenElseNode = std::make_shared<IfNode>(ifCondition1, thenBlock, elseBranch, 1);

        // line 5
        auto xNode2 = std::make_shared<VariableNode>("x", 5);
        auto lastReadStatement = std::make_shared<ReadNode>(iNode2, 5);

        auto procStmtList = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({ifThenElseNode, lastReadStatement}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStmtList);
        cfg.buildCFG(proc);
        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}},
            {2, {5}},
            {3, {4, 5}},
            {4, {3}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION("Build 2 whiles then statement in else and immediately follows a read CFG correctly") {
        /*
        01 if (i == 1) then {
        02      read y; } else {
        03    while (z != 2) {
        04        read i;}
        05      while (x !=1) {
        06      read x;}
        07  read a;} // end of else
        08  read x;
        */
        // ASTNode for if condition (i == 1)
        auto iNode = std::make_shared<VariableNode>("i", 1);
        auto constNode1 = std::make_shared<ConstantNode>("1", 1);
        auto ifCondition1 = std::make_shared<ExpressionNode>(EQUAL_CHECK, iNode, constNode1, 1);

        // ASTNode for then block (read y)
        auto yNode = std::make_shared<VariableNode>("y", 2);
        auto readYNode = std::make_shared<ReadNode>(yNode, 2);
        auto thenBlock = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readYNode}));

        // ASTNode for else block (while loop)
        auto zNode = std::make_shared<VariableNode>("z", 3);
        auto constNode2 = std::make_shared<ConstantNode>("2", 3);
        auto whileCondition = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode, constNode2, 3);

        // ASTNode for read i inside while loop
        auto iNode2 = std::make_shared<VariableNode>("i", 4);
        auto readINode = std::make_shared<ReadNode>(iNode2, 4);
        auto whileBlock = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readINode}));

        auto whileNode = std::make_shared<WhileNode>(whileCondition, whileBlock, 3);

        auto zNode1 = std::make_shared<VariableNode>("z", 5);
        auto constNode21 = std::make_shared<ConstantNode>("2", 5);
        auto whileCondition1 = std::make_shared<ExpressionNode>(NOT_EQUAL_CHECK, zNode1, constNode21, 5);

        // ASTNode for read i inside while loop
        auto iNode21 = std::make_shared<VariableNode>("i", 6);
        auto readINode1 = std::make_shared<ReadNode>(iNode21, 6);
        auto whileBlock1 =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readINode1}));

        auto whileNode1 = std::make_shared<WhileNode>(whileCondition1, whileBlock1, 5);

        auto iNode22 = std::make_shared<VariableNode>("i", 7);
        auto readINode2 = std::make_shared<ReadNode>(iNode22, 7);

        // Assemble the ASTNodes into the CFG
        auto elseBranch = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({whileNode, whileNode1, readINode2}));

        auto ifThenElseNode = std::make_shared<IfNode>(ifCondition1, thenBlock, elseBranch, 1);

        // last read
        auto iNode23 = std::make_shared<VariableNode>("i", 8);
        auto readINode3 = std::make_shared<ReadNode>(iNode23, 8);

        auto procStmtList = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({ifThenElseNode, readINode3}));

        auto proc = std::make_shared<ProcedureNode>("proc", procStmtList);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}}, {2, {8}}, {3, {4, 5}}, {4, {3}}, {5, {6, 7}}, {6, {5}}, {7, {8}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION(
        "Build 3 whiles, 1 conditional, and 1 statement in else followed by another statement outside the "
        "conditional") {
        /*
       1 procedure parent {if (y7 < 10) then {
       2                read z;
       3             } else {while (z10 > 1) {
       4                     print z10;
       5                     read z1; }
       6                 while (x2 < x3) {
       7                     print a; }
       8                 while (x2 < x4) {
       9                     read x; }
       10                 if (x4 == 1) then {
       11                     print x4;
       12                 } else { print c2;
       13                     print x2; }
       14                 print c2; }
       15             read x11;
        }

        */
        // if (y7 < 10) 1
        auto y7Node = std::make_shared<VariableNode>("y7", 1);
        auto constNode1 = std::make_shared<ConstantNode>("10", 1);
        auto outerCondition = std::make_shared<ExpressionNode>(LESS_THAN, y7Node, constNode1, 1);

        // read z 2
        auto zNode = std::make_shared<VariableNode>("z", 2);
        auto readZNode = std::make_shared<ReadNode>(zNode, 2);
        auto thenNode = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readZNode}));

        //  while (z10 > 1) 3
        auto z10Node = std::make_shared<VariableNode>("z10", 3);
        auto constNode2 = std::make_shared<ConstantNode>("1", 3);
        auto whileCondition1 = std::make_shared<ExpressionNode>(GREATER_THAN, z10Node, constNode2, 3);

        // print z10; 4
        auto z10Node2 = std::make_shared<VariableNode>("z10", 4);
        auto printNode = std::make_shared<PrintNode>(z10Node2, 4);

        // read z1; 5
        auto z1Node = std::make_shared<VariableNode>("z1", 5);
        auto readZ1Node = std::make_shared<ReadNode>(z1Node, 5);

        // while (x2 < x3) 6
        auto x2Node = std::make_shared<VariableNode>("x2", 6);
        auto x3Node = std::make_shared<VariableNode>("x3", 6);
        auto whileCondition2 = std::make_shared<ExpressionNode>(LESS_THAN, x2Node, x3Node, 6);

        // print a; 7
        auto aNode = std::make_shared<VariableNode>("a", 7);
        auto printANode = std::make_shared<PrintNode>(aNode, 7);

        // while (x2 < x4)  8
        auto x2Node1 = std::make_shared<VariableNode>("x2", 8);
        auto x4Node = std::make_shared<VariableNode>("x4", 8);
        auto whileCondition3 = std::make_shared<ExpressionNode>(LESS_THAN, x2Node1, x4Node, 8);

        // read x 9
        auto xNode = std::make_shared<VariableNode>("x4", 9);
        auto readXNode = std::make_shared<ReadNode>(xNode, 9);

        //  if (x4 == 1) 10
        auto constNode3 = std::make_shared<ConstantNode>("1", 10);
        auto x4Node1 = std::make_shared<VariableNode>("x4", 10);

        auto innerCondition = std::make_shared<ExpressionNode>(EQUAL_CHECK, x4Node1, constNode3, 10);

        // print x4 11
        auto x4Node2 = std::make_shared<VariableNode>("x4", 11);
        auto printX4Node = std::make_shared<PrintNode>(x4Node2, 11);
        auto innerThenNode =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({printX4Node}));
        //  print c2 12
        auto c2Node = std::make_shared<VariableNode>("c2", 12);

        auto printC2Node = std::make_shared<PrintNode>(c2Node, 12);

        // print x2 13
        auto x2Node2 = std::make_shared<VariableNode>("x2", 13);
        auto printX2Node = std::make_shared<PrintNode>(x2Node2, 13);
        auto innerElseNode = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({printC2Node, printX2Node}));

        //  print c2 14
        auto c2Node3 = std::make_shared<VariableNode>("c2", 14);
        auto printC2Node2 = std::make_shared<PrintNode>(c2Node3, 14);

        //  read x11 15
        auto x11Node = std::make_shared<VariableNode>("c2", 15);
        auto readX11Node = std::make_shared<ReadNode>(x11Node, 15);

        // Construct CFG
        auto whileBlock1 =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({printNode, readZ1Node}));
        auto whileNode1 = std::make_shared<WhileNode>(whileCondition1, whileBlock1, 3);

        auto whileBlock2 =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({printANode}));
        auto whileNode2 = std::make_shared<WhileNode>(whileCondition2, whileBlock2, 6);

        auto whileBlock3 =
            std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readXNode}));
        auto whileNode3 = std::make_shared<WhileNode>(whileCondition3, whileBlock3, 8);

        auto innerConditional = std::make_shared<IfNode>(innerCondition, innerThenNode, innerElseNode, 10);

        auto outerElse = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>(
            {whileNode1, whileNode2, whileNode3, innerConditional, printC2Node2}));
        auto outerThen = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({readZNode}));
        auto outerConditional = std::make_shared<IfNode>(outerCondition, outerThen, outerElse, 1);

        auto statements = std::make_shared<StatementListNode>(
            std::vector<std::shared_ptr<StatementNode>>({outerConditional, readX11Node}));

        auto proc = std::make_shared<ProcedureNode>("proc", statements);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}},  {2, {15}}, {3, {4, 6}},    {4, {5}},   {5, {3}},   {6, {7, 8}}, {7, {6}},
            {8, {9, 10}}, {9, {8}},  {10, {11, 12}}, {11, {14}}, {12, {13}}, {13, {14}},  {14, {15}}};


        REQUIRE(cfg.parentToChildMap == expected);
    }
}
