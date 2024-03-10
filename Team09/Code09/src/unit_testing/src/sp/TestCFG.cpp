
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

        for (const auto& pair : cfg.parentToChildMap) {
            std::cout << pair.first << ": ";
            const std::vector<int>& vec = pair.second;
            for (size_t i = 0; i < vec.size(); ++i) {
                std::cout << vec[i];
                if (i < vec.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        REQUIRE(cfg.parentToChildMap == expected);
    }
}
