
#include <memory>

#include "catch.hpp"
#include "sp/ast/AstNode.h"
#include "sp/cfg/Cfg.h"

using namespace std;  // NOLINT

TEST_CASE("CFG Build Tests") {
    CFG cfg;

    SECTION("Build if CFG correctly") {
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        childrens[1].push_back(iNodes[1]);
        childrens[1].push_back(constNodes[1]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[1], 1);

        childrens[2].push_back(xNodes[2]);
        childrens[2].push_back(constNodes[2]);
        auto ifConditionStatement = std::make_shared<ASTNode>("", "assign", childrens[2], 2);
        childrens[18].push_back(ifConditionStatement);
        auto ifConditionStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[18], 0);
        childrens[3].push_back(zNodes[3]);
        childrens[3].push_back(constNodes[3]);

        auto thenStatements = std::make_shared<ASTNode>("", "assign", childrens[3], 3);
        childrens[19].push_back(thenStatements);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[19], 0);
        childrens[13].push_back(ifCondition);
        childrens[13].push_back(ifConditionStmtList);
        childrens[13].push_back(thenStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[13], 1);
        childrens[16].push_back(ifNode);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[16], 0);

        childrens[17].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[17], 0);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {{1, {2, 3}}};

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while CFG correctly") {
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        childrens[1].push_back(iNodes[1]);
        childrens[1].push_back(constNodes[1]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[1], 1);

        childrens[2].push_back(xNodes[2]);
        childrens[2].push_back(yNodes[2]);
        auto whileStatement1 = std::make_shared<ASTNode>("", "assign", childrens[2], 2);

        childrens[3].push_back(iNodes[3]);
        auto whileStatement2 = std::make_shared<ASTNode>("", "read", childrens[3], 3);

        childrens[4].push_back(iNodes[4]);
        childrens[4].push_back(constNodes[4]);
        auto whileStatement3 = std::make_shared<ASTNode>("", "assign", childrens[4], 4);

        childrens[14].push_back(whileStatement1);
        childrens[14].push_back(whileStatement2);
        childrens[14].push_back(whileStatement3);
        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[14], 0);

        childrens[15].push_back(whileLoopCondition);
        childrens[15].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[15], 1);
        childrens[16].push_back(whileNode);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[16], 0);

        childrens[17].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[17], 0);
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
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        childrens[1].push_back(xNodes[1]);
        childrens[1].push_back(constNodes[1]);
        auto xAssignToZero = std::make_shared<ASTNode>("", "assign", childrens[1], 1);

        childrens[2].push_back(iNodes[2]);
        childrens[2].push_back(constNodes[2]);
        auto iAssignTo5 = std::make_shared<ASTNode>("", "assign", childrens[2], 2);

        childrens[3].push_back(iNodes[3]);
        childrens[3].push_back(constNodes[3]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[3], 3);

        childrens[4].push_back(xNodes[4]);
        childrens[4].push_back(yNodes[4]);
        auto whileStatement1 = std::make_shared<ASTNode>("", "assign", childrens[4], 4);

        childrens[5].push_back(iNodes[5]);
        auto whileStatement2 = std::make_shared<ASTNode>("", "read", childrens[5], 5);

        childrens[6].push_back(iNodes[6]);
        childrens[6].push_back(constNodes[6]);
        auto whileStatement3 = std::make_shared<ASTNode>("", "assign", childrens[6], 6);

        childrens[7].push_back(iNodes[7]);
        childrens[7].push_back(constNodes[7]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[7], 7);

        childrens[8].push_back(xNodes[8]);
        childrens[8].push_back(constNodes[8]);
        auto ifConditionStatement = std::make_shared<ASTNode>("", "assign", childrens[8], 8);
        childrens[18].push_back(ifConditionStatement);
        auto ifConditionStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[18], 0);

        childrens[9].push_back(zNodes[9]);
        childrens[9].push_back(constNodes[9]);

        auto thenStatements = std::make_shared<ASTNode>("", "assign", childrens[9], 9);
        childrens[19].push_back(thenStatements);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[19], 0);

        childrens[10].push_back(zNodes[10]);
        childrens[10].push_back(constNodes[10]);

        auto zEquals3 = std::make_shared<ASTNode>("", "assign", childrens[10], 10);

        childrens[11].push_back(yNodes[11]);
        childrens[11].push_back(constNodes[11]);

        auto yEquals4 = std::make_shared<ASTNode>("", "assign", childrens[11], 11);

        childrens[12].push_back(yNodes[12]);
        childrens[12].push_back(constNodes[12]);

        auto xEquals5 = std::make_shared<ASTNode>("", "assign", childrens[12], 12);

        childrens[13].push_back(ifCondition);
        childrens[13].push_back(ifConditionStmtList);
        childrens[13].push_back(thenStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[13], 7);

        childrens[14].push_back(whileStatement1);
        childrens[14].push_back(whileStatement2);
        childrens[14].push_back(whileStatement3);

        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[14], 0);
        childrens[15].push_back(whileLoopCondition);
        childrens[15].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[15], 3);

        childrens[16].push_back(xAssignToZero);
        childrens[16].push_back(iAssignTo5);
        childrens[16].push_back(whileNode);
        childrens[16].push_back(ifNode);
        childrens[16].push_back(zEquals3);
        childrens[16].push_back(yEquals4);
        childrens[16].push_back(xEquals5);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[16], 0);

        childrens[17].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[17], 0);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {{1, {2}},  {2, {3}},   {3, {4, 7}}, {4, {5}},
                                                              {5, {6}},  {6, {3}},   {7, {8, 9}}, {8, {10}},
                                                              {9, {10}}, {10, {11}}, {11, {12}}};

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while in if CFG correctly") {
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        /*
        01 if (i == 1) then {
        02    while (z != 2) {
        03        read i;
                }
            } else {
        04    read y;
            }
        */

        childrens[1].push_back(iNodes[1]);
        childrens[1].push_back(constNodes[1]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[1], 1);

        childrens[2].push_back(zNodes[2]);
        childrens[2].push_back(constNodes[2]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[2], 2);

        childrens[3].push_back(iNodes[3]);
        auto whileStatement1 = std::make_shared<ASTNode>("", "read", childrens[3], 3);

        childrens[5].push_back(whileStatement1);

        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[5], 0);
        childrens[6].push_back(whileLoopCondition);
        childrens[6].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[6], 2);

        childrens[7].push_back(whileNode);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[7], 0);

        childrens[4].push_back(yNodes[4]);
        auto elseStatement = std::make_shared<ASTNode>("", "read", childrens[4], 4);

        childrens[8].push_back(elseStatement);
        auto elseStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[8], 0);

        childrens[9].push_back(ifCondition);
        childrens[9].push_back(thenStmtList);
        childrens[9].push_back(elseStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[9], 1);
        childrens[10].push_back(ifNode);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[10], 0);

        childrens[11].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[11], 0);
        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 4}},
            {2, {3}},
            {3, {2}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
    }

    SECTION("Build if in while CFG correctly") {
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        /*
        01    while (z != 2) {
        02        if (i == 0) then {
        03            read i;
                } else {
        04            read a;
                }
        */

        childrens[1].push_back(zNodes[1]);
        childrens[1].push_back(constNodes[1]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[1], 1);

        childrens[2].push_back(iNodes[2]);
        childrens[2].push_back(constNodes[2]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[2], 2);

        childrens[3].push_back(yNodes[3]);
        auto readStmt = std::make_shared<ASTNode>("", "read", childrens[3], 3);

        childrens[4].push_back(readStmt);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[4], 0);

        childrens[5].push_back(yNodes[5]);
        auto readStmt2 = std::make_shared<ASTNode>("", "read", childrens[5], 4);

        childrens[11].push_back(readStmt2);

        auto elseStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[11], 0);

        childrens[6].push_back(ifCondition);
        childrens[6].push_back(thenStmtList);
        childrens[6].push_back(elseStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[6], 2);
        childrens[7].push_back(ifNode);
        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[7], 0);

        childrens[8].push_back(whileLoopCondition);
        childrens[8].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[8], 1);

        childrens[9].push_back(whileNode);
        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[9], 0);

        childrens[10].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[10], 0);
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
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

        childrens[1].push_back(iNodes[1]);
        childrens[1].push_back(constNodes[1]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[1], 1);
        childrens[2].push_back(yNodes[2]);
        auto readStmt = std::make_shared<ASTNode>("", "read", childrens[2], 2);

        childrens[3].push_back(readStmt);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[3], 0);

        childrens[4].push_back(yNodes[4]);
        auto readStmt2 = std::make_shared<ASTNode>("", "read", childrens[4], 3);

        childrens[5].push_back(readStmt2);

        auto elseStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[5], 0);

        childrens[6].push_back(ifCondition);
        childrens[6].push_back(thenStmtList);
        childrens[6].push_back(elseStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[6], 1);

        childrens[7].push_back(zNodes[7]);
        childrens[7].push_back(constNodes[7]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[7], 4);

        childrens[8].push_back(yNodes[8]);
        auto readStmt3 = std::make_shared<ASTNode>("", "read", childrens[8], 5);

        childrens[9].push_back(readStmt3);
        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[9], 0);

        childrens[10].push_back(whileLoopCondition);
        childrens[10].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[10], 4);

        childrens[11].push_back(ifNode);
        childrens[11].push_back(whileNode);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[11], 0);
        childrens[12].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[12], 0);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}}, {2, {4}}, {3, {4}}, {4, {5}}, {5, {4}},
        };

        REQUIRE(cfg.parentToChildMap == expected);
        cfg.parentToChildMap = {};
    }

    SECTION("Build while in if then something CFG correctly") {
        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i < 20; i++) {
            zNodes.push_back(std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            xNodes.push_back(std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            yNodes.push_back(std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            constNodes.push_back(
                std::make_shared<ASTNode>(std::to_string(i), "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            iNodes.push_back(std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, i));
            childrens[i] = {};
        }

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

        childrens[1].push_back(iNodes[1]);
        childrens[1].push_back(constNodes[1]);

        auto ifCondition = std::make_shared<ASTNode>("", "==", childrens[1], 1);

        childrens[2].push_back(zNodes[2]);
        childrens[2].push_back(constNodes[2]);
        auto whileLoopCondition = std::make_shared<ASTNode>("", "!=", childrens[2], 2);

        childrens[3].push_back(iNodes[3]);
        auto whileStatement1 = std::make_shared<ASTNode>("", "read", childrens[3], 3);

        childrens[5].push_back(whileStatement1);

        auto whileStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[5], 0);
        childrens[6].push_back(whileLoopCondition);
        childrens[6].push_back(whileStmtList);

        auto whileNode = std::make_shared<ASTNode>("", "while", childrens[6], 2);

        childrens[7].push_back(whileNode);
        auto thenStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[7], 0);

        childrens[4].push_back(yNodes[4]);
        auto elseStatement = std::make_shared<ASTNode>("", "read", childrens[4], 4);

        childrens[8].push_back(elseStatement);
        auto elseStmtList = std::make_shared<ASTNode>("", "stmtList", childrens[8], 0);

        childrens[9].push_back(ifCondition);
        childrens[9].push_back(thenStmtList);
        childrens[9].push_back(elseStmtList);
        auto ifNode = std::make_shared<ASTNode>("", "if", childrens[9], 1);

        childrens[12].push_back(yNodes[12]);
        auto readStatement = std::make_shared<ASTNode>("", "read", childrens[12], 5);
        childrens[10].push_back(ifNode);
        childrens[10].push_back(readStatement);

        auto procStatementList = std::make_shared<ASTNode>("", "stmtList", childrens[10], 0);

        childrens[11].push_back(procStatementList);
        auto proc = std::make_shared<ASTNode>("Second", "proc", childrens[11], 0);
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
        auto iNode = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        auto constNode1 = std::make_shared<ASTNode>("1", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        std::vector<std::shared_ptr<ASTNode>> children1 = {iNode, constNode1};
        auto ifCondition1 = std::make_shared<ASTNode>("", "==", children1, 1);

        // ASTNode for then block (read y)
        auto yNode = std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, 2);
        std::vector<std::shared_ptr<ASTNode>> readYChildren = {yNode};
        auto readYNode = std::make_shared<ASTNode>("", "read", readYChildren, 2);
        std::vector<std::shared_ptr<ASTNode>> thenStmts = {readYNode};
        auto thenBlock = std::make_shared<ASTNode>("", "stmtList", thenStmts, 0);

        // ASTNode for else block (while loop)
        auto zNode = std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        auto constNode2 = std::make_shared<ASTNode>("2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        std::vector<std::shared_ptr<ASTNode>> children2 = {zNode, constNode2};
        auto whileCondition = std::make_shared<ASTNode>("", "!=", children2, 3);

        // ASTNode for read i inside while loop
        auto iNode2 = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 4);
        std::vector<std::shared_ptr<ASTNode>> readChild = {iNode2};
        auto readINode = std::make_shared<ASTNode>("", "read", readChild, 4);
        std::vector<std::shared_ptr<ASTNode>> whileStmts = {readINode};
        auto whileBlock = std::make_shared<ASTNode>("", "stmtList", whileStmts, 0);

        std::vector<std::shared_ptr<ASTNode>> whileChildren = {whileCondition, whileBlock};
        auto whileNode = std::make_shared<ASTNode>("", "while", whileChildren, 3);

        // Assemble the ASTNodes into the CFG
        std::vector<std::shared_ptr<ASTNode>> elseBranchChildren = {whileNode};
        auto elseBranch = std::make_shared<ASTNode>("", "stmtList", elseBranchChildren, 0);

        std::vector<std::shared_ptr<ASTNode>> ifThenElseChildren = {ifCondition1, thenBlock, elseBranch};
        auto ifThenElseNode = std::make_shared<ASTNode>("", "if", ifThenElseChildren, 1);

        // line 5
        auto xNode2 = std::make_shared<ASTNode>("x", "var", std::vector<std::shared_ptr<ASTNode>>{}, 5);
        std::vector<std::shared_ptr<ASTNode>> readChild2 = {iNode2};
        auto lastReadStatement = std::make_shared<ASTNode>("", "read", readChild2, 5);

        std::vector<std::shared_ptr<ASTNode>> stmtListChildren = {ifThenElseNode, lastReadStatement};
        auto procStmtList = std::make_shared<ASTNode>("", "stmtList", stmtListChildren, 0);

        std::vector<std::shared_ptr<ASTNode>> procChildren = {procStmtList};
        auto proc = std::make_shared<ASTNode>("Second", "proc", procChildren, 0);
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
        auto iNode = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        auto constNode1 = std::make_shared<ASTNode>("1", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        std::vector<std::shared_ptr<ASTNode>> children1 = {iNode, constNode1};
        auto ifCondition1 = std::make_shared<ASTNode>("", "==", children1, 1);

        // ASTNode for then block (read y)
        auto yNode = std::make_shared<ASTNode>("y", "var", std::vector<std::shared_ptr<ASTNode>>{}, 2);
        std::vector<std::shared_ptr<ASTNode>> readYChildren = {yNode};
        auto readYNode = std::make_shared<ASTNode>("", "read", readYChildren, 2);
        std::vector<std::shared_ptr<ASTNode>> thenStmts = {readYNode};
        auto thenBlock = std::make_shared<ASTNode>("", "stmtList", thenStmts, 0);

        // ASTNode for else block (while loop)
        auto zNode = std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        auto constNode2 = std::make_shared<ASTNode>("2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        std::vector<std::shared_ptr<ASTNode>> children2 = {zNode, constNode2};
        auto whileCondition = std::make_shared<ASTNode>("", "!=", children2, 3);

        // ASTNode for read i inside while loop
        auto iNode2 = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 4);
        std::vector<std::shared_ptr<ASTNode>> readChild = {iNode2};
        auto readINode = std::make_shared<ASTNode>("", "read", readChild, 4);
        std::vector<std::shared_ptr<ASTNode>> whileStmts = {readINode};
        auto whileBlock = std::make_shared<ASTNode>("", "stmtList", whileStmts, 0);

        std::vector<std::shared_ptr<ASTNode>> whileChildren = {whileCondition, whileBlock};
        auto whileNode = std::make_shared<ASTNode>("", "while", whileChildren, 3);

        auto zNode1 = std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, 5);
        auto constNode21 = std::make_shared<ASTNode>("2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 5);
        std::vector<std::shared_ptr<ASTNode>> children21 = {zNode1, constNode21};
        auto whileCondition1 = std::make_shared<ASTNode>("", "!=", children21, 5);

        // ASTNode for read i inside while loop
        auto iNode21 = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 6);
        std::vector<std::shared_ptr<ASTNode>> readChild1 = {iNode21};
        auto readINode1 = std::make_shared<ASTNode>("", "read", readChild1, 6);
        std::vector<std::shared_ptr<ASTNode>> whileStmts1 = {readINode1};
        auto whileBlock1 = std::make_shared<ASTNode>("", "stmtList", whileStmts1, 0);

        std::vector<std::shared_ptr<ASTNode>> whileChildren1 = {whileCondition1, whileBlock1};
        auto whileNode1 = std::make_shared<ASTNode>("", "while", whileChildren1, 5);

        auto iNode22 = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 7);
        std::vector<std::shared_ptr<ASTNode>> readChild2 = {iNode22};
        auto readINode2 = std::make_shared<ASTNode>("", "read", readChild2, 7);

        // Assemble the ASTNodes into the CFG
        std::vector<std::shared_ptr<ASTNode>> elseBranchChildren = {whileNode, whileNode1, readINode2};
        auto elseBranch = std::make_shared<ASTNode>("", "stmtList", elseBranchChildren, 0);

        std::vector<std::shared_ptr<ASTNode>> ifThenElseChildren = {ifCondition1, thenBlock, elseBranch};
        auto ifThenElseNode = std::make_shared<ASTNode>("", "if", ifThenElseChildren, 1);

        // last read
        auto iNode23 = std::make_shared<ASTNode>("i", "var", std::vector<std::shared_ptr<ASTNode>>{}, 8);
        std::vector<std::shared_ptr<ASTNode>> readChild3 = {iNode23};
        auto readINode3 = std::make_shared<ASTNode>("", "read", readChild3, 8);

        std::vector<std::shared_ptr<ASTNode>> stmtListChildren = {ifThenElseNode, readINode3};
        auto procStmtList = std::make_shared<ASTNode>("", "stmtList", stmtListChildren, 0);

        std::vector<std::shared_ptr<ASTNode>> procChildren = {procStmtList};
        auto proc = std::make_shared<ASTNode>("Second", "proc", procChildren, 0);

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
        auto y7Node = std::make_shared<ASTNode>("y7", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        auto constNode1 = std::make_shared<ASTNode>("10", "var", std::vector<std::shared_ptr<ASTNode>>{}, 1);
        std::vector<std::shared_ptr<ASTNode>> children1 = {y7Node, constNode1};
        auto outerCondition = std::make_shared<ASTNode>("", "<", children1, 1);

        // read z 2
        auto zNode = std::make_shared<ASTNode>("z", "var", std::vector<std::shared_ptr<ASTNode>>{}, 2);
        auto readZNode = std::make_shared<ASTNode>("", "read", std::vector<std::shared_ptr<ASTNode>>{zNode}, 2);
        auto thenNode = std::make_shared<ASTNode>("", "stmtList", std::vector<std::shared_ptr<ASTNode>>{readZNode}, 0);

        //  while (z10 > 1) 3
        auto z10Node = std::make_shared<ASTNode>("z10", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        auto constNode2 = std::make_shared<ASTNode>("1", "var", std::vector<std::shared_ptr<ASTNode>>{}, 3);
        std::vector<std::shared_ptr<ASTNode>> children2 = {z10Node, constNode2};
        auto whileCondition1 = std::make_shared<ASTNode>("", ">", children2, 3);

        // print z10; 4
        auto z10Node2 = std::make_shared<ASTNode>("z10", "var", std::vector<std::shared_ptr<ASTNode>>{}, 4);
        auto printNode = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{z10Node2}, 4);

        // read z1; 5
        auto z1Node = std::make_shared<ASTNode>("z1", "var", std::vector<std::shared_ptr<ASTNode>>{}, 5);
        auto readZ1Node = std::make_shared<ASTNode>("", "read", std::vector<std::shared_ptr<ASTNode>>{z1Node}, 5);

        // while (x2 < x3) 6
        auto x2Node = std::make_shared<ASTNode>("x2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 6);
        auto x3Node = std::make_shared<ASTNode>("x3", "var", std::vector<std::shared_ptr<ASTNode>>{}, 6);
        std::vector<std::shared_ptr<ASTNode>> children3 = {x2Node, x3Node};
        auto whileCondition2 = std::make_shared<ASTNode>("", "<", children3, 6);

        // print a; 7
        auto aNode = std::make_shared<ASTNode>("a", "var", std::vector<std::shared_ptr<ASTNode>>{}, 7);
        auto printANode = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{aNode}, 7);

        // while (x2 < x4)  8
        auto x2Node1 = std::make_shared<ASTNode>("x2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 8);
        auto x4Node = std::make_shared<ASTNode>("x4", "var", std::vector<std::shared_ptr<ASTNode>>{}, 8);
        std::vector<std::shared_ptr<ASTNode>> children4 = {x2Node1, x4Node};
        auto whileCondition3 = std::make_shared<ASTNode>("", "<", children4, 8);

        // read x 9
        auto xNode = std::make_shared<ASTNode>("x4", "var", std::vector<std::shared_ptr<ASTNode>>{}, 9);
        auto readXNode = std::make_shared<ASTNode>("", "read", std::vector<std::shared_ptr<ASTNode>>{xNode}, 9);

        //  if (x4 == 1) 10
        auto constNode3 = std::make_shared<ASTNode>("1", "var", std::vector<std::shared_ptr<ASTNode>>{}, 10);
        auto x4Node1 = std::make_shared<ASTNode>("x4", "var", std::vector<std::shared_ptr<ASTNode>>{}, 10);

        std::vector<std::shared_ptr<ASTNode>> children5 = {x4Node1, constNode3};
        auto innerCondition = std::make_shared<ASTNode>("", "==", children5, 10);

        // print x4 11
        auto x4Node2 = std::make_shared<ASTNode>("x4", "var", std::vector<std::shared_ptr<ASTNode>>{}, 11);
        auto printX4Node = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{x4Node2}, 11);
        auto innerThenNode =
            std::make_shared<ASTNode>("", "stmtList", std::vector<std::shared_ptr<ASTNode>>{printX4Node}, 0);
        //  print c2 12
        auto c2Node = std::make_shared<ASTNode>("c2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 12);

        auto printC2Node = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{c2Node}, 12);

        // print x2 13
        auto x2Node2 = std::make_shared<ASTNode>("x2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 13);
        auto printX2Node = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{x2Node2}, 13);
        auto innerElseNode = std::make_shared<ASTNode>(
            "", "stmtList", std::vector<std::shared_ptr<ASTNode>>{printC2Node, printX2Node}, 0);

        //  print c2 14
        auto c2Node3 = std::make_shared<ASTNode>("c2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 14);
        auto printC2Node2 = std::make_shared<ASTNode>("", "print", std::vector<std::shared_ptr<ASTNode>>{c2Node3}, 14);

        //  read x11 15
        auto x11Node = std::make_shared<ASTNode>("c2", "var", std::vector<std::shared_ptr<ASTNode>>{}, 15);
        auto readX11Node = std::make_shared<ASTNode>("", "read", std::vector<std::shared_ptr<ASTNode>>{x11Node}, 15);

        // Construct CFG
        std::vector<std::shared_ptr<ASTNode>> whileStmts1 = {printNode, readZ1Node};
        auto whileBlock1 = std::make_shared<ASTNode>("", "stmtList", whileStmts1, 0);
        auto whileNode1 = std::make_shared<ASTNode>(
            "", "while", std::vector<std::shared_ptr<ASTNode>>{whileCondition1, whileBlock1}, 3);

        std::vector<std::shared_ptr<ASTNode>> whileStmts2 = {printANode};
        auto whileBlock2 = std::make_shared<ASTNode>("", "stmtList", whileStmts2, 0);
        auto whileNode2 = std::make_shared<ASTNode>(
            "", "while", std::vector<std::shared_ptr<ASTNode>>{whileCondition2, whileBlock2}, 6);

        std::vector<std::shared_ptr<ASTNode>> whileStmts3 = {readXNode};
        auto whileBlock3 = std::make_shared<ASTNode>("", "stmtList", whileStmts3, 0);
        auto whileNode3 = std::make_shared<ASTNode>(
            "", "while", std::vector<std::shared_ptr<ASTNode>>{whileCondition3, whileBlock3}, 8);

        auto innerConditional = std::make_shared<ASTNode>(
            "", "if", std::vector<std::shared_ptr<ASTNode>>{innerCondition, innerThenNode, innerElseNode}, 10);

        auto outerElse = std::make_shared<ASTNode>(
            "", "stmtList",
            std::vector<std::shared_ptr<ASTNode>>{whileNode1, whileNode2, whileNode3, innerConditional, printC2Node2},
            0);
        auto outerThen = std::make_shared<ASTNode>("", "stmtList", std::vector<std::shared_ptr<ASTNode>>{readZNode}, 0);
        auto outerConditional = std::make_shared<ASTNode>(
            "", "if", std::vector<std::shared_ptr<ASTNode>>{outerCondition, outerThen, outerElse}, 1);

        auto statements = std::make_shared<ASTNode>(
            "", "stmtList", std::vector<std::shared_ptr<ASTNode>>{outerConditional, readX11Node}, 0);

        auto proc = std::make_shared<ASTNode>("parent", "proc", std::vector<std::shared_ptr<ASTNode>>{statements}, 0);

        cfg.buildCFG(proc);

        std::unordered_map<int, std::vector<int>> expected = {
            {1, {2, 3}},  {2, {15}}, {3, {4, 6}},    {4, {5}},   {5, {3}},   {6, {7, 8}}, {7, {6}},
            {8, {9, 10}}, {9, {8}},  {10, {11, 12}}, {11, {14}}, {12, {13}}, {13, {14}},  {14, {15}}};

        REQUIRE(cfg.parentToChildMap == expected);
    }
}
