
#include <memory>

#include "catch.hpp"
#include "sp/ast/Ast.h"
#include "sp/cfg/Cfg.h"
#include "utils.h"

using namespace std;  // NOLINT

TEST_CASE("CFG Build Tests") {
    AST ast;
    CFG cfg;

    SECTION("Build while then if CFG correctly") {
        /*
        procedure Second {
            x = 0;
            i = 5;
            while (i!=0) {
                x = y;
                read a;
                i = 1;
            }
            if (x==1) then {
                 x = 1; }
            else {
                z = 1;
            }
            z = 3;
            y = 4;
             x = 5; }
        */

        std::vector<std::shared_ptr<ASTNode>> childrens[20];

        std::vector<std::shared_ptr<ASTNode>> constNodes = {};
        std::vector<std::shared_ptr<ASTNode>> xNodes = {};
        std::vector<std::shared_ptr<ASTNode>> yNodes = {};
        std::vector<std::shared_ptr<ASTNode>> zNodes = {};
        std::vector<std::shared_ptr<ASTNode>> iNodes = {};

        for (int i = 0; i <= 12; i++) {
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
    }
}
