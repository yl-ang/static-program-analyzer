#include <memory>
#include <tuple>

#include "catch.hpp"
#include "sp/ast/grammar_nodes/ConstantNode.h"
#include "sp/ast/grammar_nodes/statements/AssignmentNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/WhileNode.h"
#include "sp/de/AstVisitor.h"
#include "sp/de/DesignExtractor.h"

TEST_CASE("Entity Extractor Tests") {
    /*
        procedure proc1 {
            01 x = y + 2;
            02 read a;
            03 while ((c < d) && (e > f)) {
            04     g = h;
            }
        }
        procedure proc2 {
            05 print z;
            06 if (!(i < j)) then {
            07     i = j;
            08     x = y + 2;
            } else {
            09     a = b;
            10     call proc1;
            }
        }
        procedure proc3 {
            11 call proc2;
            12 call proc1;
            13 x = x - 1;
        }
    */
    // 01 x = y + 2;
    std::shared_ptr<VariableNode> ANode_x = std::make_shared<VariableNode>(VariableNode("x", 1));
    std::shared_ptr<VariableNode> ANode_y = std::make_shared<VariableNode>(VariableNode("y", 1));
    std::shared_ptr<ConstantNode> ANode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 1));
    std::shared_ptr<ExpressionNode> ANode_ADD =
        std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, ANode_y, ANode_2, 1));
    auto assignNode = std::make_shared<AssignmentNode>(ANode_x, ANode_ADD, 1);

    // 02 read a;
    std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 2));
    auto readNode = std::make_shared<ReadNode>(RNode_a, 2);

    // 03 while ((c < d) && (e > f)) {
    // 04     g = h;
    //    }
    std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 3));
    std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 3));
    std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 3));
    std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 3));
    std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 4));
    std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 4));

    std::vector<std::shared_ptr<StatementNode>> WChildren = {};

    auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 3);
    auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 3);
    auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 3);

    auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 4);
    WChildren.push_back(Wassign);

    auto WstmtList = std::make_shared<StatementListNode>(WChildren);
    auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 3);

    std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
    childrenProc1.push_back(assignNode);
    childrenProc1.push_back(readNode);
    childrenProc1.push_back(whileNode);

    auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
    auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

    // 05 print z;
    std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 5));
    auto printNode = std::make_shared<PrintNode>(PNode_z, 5);

    // 06 if (!(i < j)) then {
    // 07     i = j;
    // 08     x = y + 2;
    //    } else {
    // 09     a = b;
    // 10     call proc1;
    //    }
    std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 6));
    std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 6));
    std::shared_ptr<VariableNode> INode_i1 = std::make_shared<VariableNode>(VariableNode("i", 7));
    std::shared_ptr<VariableNode> INode_j1 = std::make_shared<VariableNode>(VariableNode("j", 7));
    std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 8));
    std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 8));
    std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 8));
    std::shared_ptr<ExpressionNode> INode_ADD =
        std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 8));
    std::shared_ptr<VariableNode> ENode_a = std::make_shared<VariableNode>(VariableNode("a", 9));
    std::shared_ptr<VariableNode> ENode_b = std::make_shared<VariableNode>(VariableNode("b", 9));

    auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 6);
    auto InotNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::NOT, IlessThan, 6);

    std::vector<std::shared_ptr<StatementNode>> IChildren = {};
    std::vector<std::shared_ptr<StatementNode>> EChildren = {};

    auto Iassign = std::make_shared<AssignmentNode>(INode_i1, INode_j1, 7);
    auto Iassign2 = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 8);
    IChildren.push_back(Iassign);
    IChildren.push_back(Iassign2);

    auto Eassign = std::make_shared<AssignmentNode>(ENode_a, ENode_b, 9);
    auto Ecall = std::make_shared<CallNode>("proc1", 10);
    EChildren.push_back(Eassign);
    EChildren.push_back(Ecall);

    auto IstmtList = std::make_shared<StatementListNode>(IChildren);
    auto EstmtList = std::make_shared<StatementListNode>(EChildren);
    auto ifNode = std::make_shared<IfNode>(InotNode, IstmtList, EstmtList, 6);

    std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
    childrenProc2.push_back(printNode);
    childrenProc2.push_back(ifNode);

    auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
    auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

    // 11 call proc2;
    auto callNode = std::make_shared<CallNode>("proc2", 11);

    // 12 call proc1;
    auto callNode1 = std::make_shared<CallNode>("proc1", 12);

    // 13 x = x - 1;
    std::shared_ptr<VariableNode> A1Node_x = std::make_shared<VariableNode>(VariableNode("x", 13));
    std::shared_ptr<VariableNode> A1Node_x1 = std::make_shared<VariableNode>(VariableNode("x", 13));
    std::shared_ptr<ConstantNode> A1Node_1 = std::make_shared<ConstantNode>(ConstantNode("1", 13));
    std::shared_ptr<ExpressionNode> A1Node_SUB =
        std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::SUB, A1Node_x1, A1Node_1, 13));

    auto assignNode1 = std::make_shared<AssignmentNode>(A1Node_x, A1Node_SUB, 13);

    std::vector<std::shared_ptr<StatementNode>> childrenProc3 = {};
    childrenProc3.push_back(callNode);
    childrenProc3.push_back(callNode1);
    childrenProc3.push_back(assignNode1);

    auto stmtListProc3 = std::make_shared<StatementListNode>(childrenProc3);
    auto Proc3Node = std::make_shared<ProcedureNode>("proc3", stmtListProc3);

    // Create program
    std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
    childrenProg.push_back(Proc1Node);
    childrenProg.push_back(Proc2Node);
    childrenProg.push_back(Proc3Node);

    auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

    SECTION("VARIABLEs extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::string> expectedVariables = {"x", "y", "a", "c", "d", "e", "f",
                                                             "g", "h", "z", "i", "j", "b"};
        REQUIRE(expectedVariables == designExtractor.getVariables());
    }

    SECTION("CONSTANTs extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::string> expectedConstants = {"2", "1"};
        REQUIRE(expectedConstants == designExtractor.getConstants());
    }

    SECTION("STATEMENTs extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<Stmt> expectedStatements = {
            Stmt{StatementType::ASSIGN, 1}, Stmt{StatementType::READ, 2},   Stmt{StatementType::WHILE, 3},
            Stmt{StatementType::ASSIGN, 4}, Stmt{StatementType::PRINT, 5},  Stmt{StatementType::IF, 6},
            Stmt{StatementType::ASSIGN, 7}, Stmt{StatementType::ASSIGN, 8}, Stmt{StatementType::ASSIGN, 9},
            Stmt{StatementType::CALL, 10},  Stmt{StatementType::CALL, 11},  Stmt{StatementType::CALL, 12},
            Stmt{StatementType::ASSIGN, 13}};
        REQUIRE(expectedStatements == designExtractor.getStatements());
    }

    SECTION("PROCEDUREs extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::string> expectedProcedures = {"proc1", "proc2", "proc3"};
        REQUIRE(expectedProcedures == designExtractor.getProcedures());
    }
}

TEST_CASE("Follows Extractor Tests") {
    /*
        procedure proc1 {
            01 read a;
            02 while ((c < d) && (e > f)) {
            03     g = h;
               }
        }
        procedure proc2 {
            04 print z;
            05 if (!(i < j)) then {
            06     x = y + 2;
               } else {
            07     a = b;
            08     call proc1;
               }
        }
    */

    // 01 read a;
    std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 1));
    auto readNode = std::make_shared<ReadNode>(RNode_a, 1);

    // 02 while ((c < d) && (e > f)) {
    // 03     g = h;
    //    }
    std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 2));
    std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 2));
    std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 2));
    std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 2));
    std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 3));
    std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 3));

    std::vector<std::shared_ptr<StatementNode>> WChildren = {};

    auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 2);
    auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 2);
    auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 2);

    auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 3);
    WChildren.push_back(Wassign);

    auto WstmtList = std::make_shared<StatementListNode>(WChildren);
    auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 2);

    std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
    childrenProc1.push_back(readNode);
    childrenProc1.push_back(whileNode);

    auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
    auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

    // 04 print z;
    std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 4));
    auto printNode = std::make_shared<PrintNode>(PNode_z, 4);

    // 05 if (!(i < j)) then {
    // 06     x = y + 2;
    //    } else {
    // 07     a = b;
    // 08     call proc1;
    //    }
    std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 5));
    std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 5));
    std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 6));
    std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 6));
    std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 6));
    std::shared_ptr<ExpressionNode> INode_ADD =
        std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 6));
    std::shared_ptr<VariableNode> ENode_a = std::make_shared<VariableNode>(VariableNode("a", 7));
    std::shared_ptr<VariableNode> ENode_b = std::make_shared<VariableNode>(VariableNode("b", 7));

    auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 5);
    auto InotNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::NOT, IlessThan, 5);

    std::vector<std::shared_ptr<StatementNode>> IChildren = {};
    std::vector<std::shared_ptr<StatementNode>> EChildren = {};

    auto Iassign = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 6);
    IChildren.push_back(Iassign);

    auto Eassign = std::make_shared<AssignmentNode>(ENode_a, ENode_b, 7);
    auto Ecall = std::make_shared<CallNode>("proc1", 8);
    EChildren.push_back(Eassign);
    EChildren.push_back(Ecall);

    auto IstmtList = std::make_shared<StatementListNode>(IChildren);
    auto EstmtList = std::make_shared<StatementListNode>(EChildren);
    auto ifNode = std::make_shared<IfNode>(InotNode, IstmtList, EstmtList, 5);

    std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
    childrenProc2.push_back(printNode);
    childrenProc2.push_back(ifNode);

    auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
    auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

    // Create program
    std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
    childrenProg.push_back(Proc1Node);
    childrenProg.push_back(Proc2Node);

    auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

    SECTION("FOLLOWS extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedFollows = {{1, 2}, {4, 5}, {7, 8}};
        REQUIRE(expectedFollows == designExtractor.getFollows());
    }
}

TEST_CASE("Parent Extractor Tests") {
    SECTION("PARENTs extracted correctly") {
        /*
            procedure proc1 {
                01 read a;
                02 while ((c < d) && (e > f)) {
                03     g = h;
                }
            }
            procedure proc2 {
                04 print z;
                05 if (!(i < j)) then {
                06     x = y + 2;
                } else {
                07     a = b;
                08     call proc1;
                }
            }
        */

        // 01 read a;
        std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 1));
        auto readNode = std::make_shared<ReadNode>(RNode_a, 1);

        // 02 while ((c < d) && (e > f)) {
        // 03     g = h;
        //    }
        std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 2));
        std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 2));
        std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 2));
        std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 2));
        std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 3));
        std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 3));

        std::vector<std::shared_ptr<StatementNode>> WChildren = {};

        auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 2);
        auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 2);
        auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 2);

        auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 3);
        WChildren.push_back(Wassign);

        auto WstmtList = std::make_shared<StatementListNode>(WChildren);
        auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 2);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(readNode);
        childrenProc1.push_back(whileNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 04 print z;
        std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 4));
        auto printNode = std::make_shared<PrintNode>(PNode_z, 4);

        // 05 if (!(i < j)) then {
        // 06     x = y + 2;
        //    } else {
        // 07     a = b;
        // 08     call proc1;
        //    }
        std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 5));
        std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 5));
        std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 6));
        std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 6));
        std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 6));
        std::shared_ptr<ExpressionNode> INode_ADD =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 6));
        std::shared_ptr<VariableNode> ENode_a = std::make_shared<VariableNode>(VariableNode("a", 7));
        std::shared_ptr<VariableNode> ENode_b = std::make_shared<VariableNode>(VariableNode("b", 7));

        auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 5);
        auto InotNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::NOT, IlessThan, 5);

        std::vector<std::shared_ptr<StatementNode>> IChildren = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren = {};

        auto Iassign = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 6);
        IChildren.push_back(Iassign);

        auto Eassign = std::make_shared<AssignmentNode>(ENode_a, ENode_b, 7);
        auto Ecall = std::make_shared<CallNode>("proc1", 8);
        EChildren.push_back(Eassign);
        EChildren.push_back(Ecall);

        auto IstmtList = std::make_shared<StatementListNode>(IChildren);
        auto EstmtList = std::make_shared<StatementListNode>(EChildren);
        auto ifNode = std::make_shared<IfNode>(InotNode, IstmtList, EstmtList, 5);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(printNode);
        childrenProc2.push_back(ifNode);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedParent = {{2, 3}, {5, 6}, {5, 7}, {5, 8}};
        REQUIRE(expectedParent == designExtractor.getParent());
    }

    SECTION("PARENTs extracted correctly - Nested IF and WHILE statements") {
        /*
            procedure proc1 {
                01 read a;
                02 while ((c < d) && (e > f)) {
                03     g = h;
                04     if (!(i < j)) then {
                05         x = y + 2;
                       } else {
                06         a = b;
                07         call proc2;
                       }
                    }
            }
            procedure proc2 {
                08 print z;
            }
        */

        // 01 read a;
        std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 1));
        auto readNode = std::make_shared<ReadNode>(RNode_a, 1);

        // 02 while ((c < d) && (e > f)) {
        // 03     g = h;
        //    }
        std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 2));
        std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 2));
        std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 2));
        std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 2));
        std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 3));
        std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 3));

        std::vector<std::shared_ptr<StatementNode>> WChildren = {};

        auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 2);
        auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 2);
        auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 2);

        auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 3);

        // 04 if (!(i < j)) then {
        // 05     x = y + 2;
        //    } else {
        // 06     a = b;
        // 07     call proc2;
        //    }
        std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 4));
        std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 4));
        std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 5));
        std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 5));
        std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 5));
        std::shared_ptr<ExpressionNode> INode_ADD =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 5));
        std::shared_ptr<VariableNode> ENode_a = std::make_shared<VariableNode>(VariableNode("a", 6));
        std::shared_ptr<VariableNode> ENode_b = std::make_shared<VariableNode>(VariableNode("b", 6));

        auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 4);
        auto InotNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::NOT, IlessThan, 4);

        std::vector<std::shared_ptr<StatementNode>> IChildren = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren = {};

        auto Iassign = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 5);
        IChildren.push_back(Iassign);

        auto Eassign = std::make_shared<AssignmentNode>(ENode_a, ENode_b, 6);
        auto Ecall = std::make_shared<CallNode>("proc2", 7);
        EChildren.push_back(Eassign);
        EChildren.push_back(Ecall);

        auto IstmtList = std::make_shared<StatementListNode>(IChildren);
        auto EstmtList = std::make_shared<StatementListNode>(EChildren);
        auto ifNode = std::make_shared<IfNode>(InotNode, IstmtList, EstmtList, 4);

        WChildren.push_back(Wassign);
        WChildren.push_back(ifNode);

        auto WstmtList = std::make_shared<StatementListNode>(WChildren);
        auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 2);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(readNode);
        childrenProc1.push_back(whileNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 08 print z;
        std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 8));
        auto printNode = std::make_shared<PrintNode>(PNode_z, 8);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(printNode);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, StmtNum>> expectedParent = {{2, 3}, {2, 4}, {4, 5}, {4, 6}, {4, 7}};
        REQUIRE(expectedParent == designExtractor.getParent());
    }
}

TEST_CASE("Modifies Extractor Tests") {
    SECTION("MODIFIES extracted correctly - READ statements") {
        /*
            procedure proc1 {
                01 read z;
            }
            procedure proc2 {
                02 read y;
            }
        */
        // 01 read z;
        std::shared_ptr<VariableNode> RNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto readNode = std::make_shared<ReadNode>(RNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(readNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 read y;
        std::shared_ptr<VariableNode> RNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto readNode1 = std::make_shared<ReadNode>(RNode_y, 2);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(readNode1);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{1, "z"}, {2, "y"}};
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("MODIFIES extracted correctly - ASSIGN statements") {
        /*
            procedure proc1 {
                01 x = 2 * (5 - y);
            }
            procedure proc2 {
                02 x = 4;
                03 z = v;
            }
        */
        // 01 x = 2 * (5 - y);
        std::shared_ptr<VariableNode> ANode_x = std::make_shared<VariableNode>(VariableNode("x", 1));
        std::shared_ptr<VariableNode> ANode_y = std::make_shared<VariableNode>(VariableNode("y", 1));
        std::shared_ptr<ConstantNode> ANode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 1));
        std::shared_ptr<ConstantNode> ANode_5 = std::make_shared<ConstantNode>(ConstantNode("5", 1));
        std::shared_ptr<ExpressionNode> ANode_SUB =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::SUB, ANode_5, ANode_y, 1));
        std::shared_ptr<ExpressionNode> ANode_MUL =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::MUL, ANode_2, ANode_SUB, 1));
        auto assignNode = std::make_shared<AssignmentNode>(ANode_x, ANode_MUL, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(assignNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 x = 4;
        std::shared_ptr<VariableNode> ANode_x1 = std::make_shared<VariableNode>(VariableNode("x", 2));
        std::shared_ptr<ConstantNode> ANode_4 = std::make_shared<ConstantNode>(ConstantNode("4", 2));
        auto assignNode2 = std::make_shared<AssignmentNode>(ANode_x1, ANode_4, 2);

        // 03 z = v;
        std::shared_ptr<VariableNode> ANode_z = std::make_shared<VariableNode>(VariableNode("z", 3));
        std::shared_ptr<VariableNode> ANode_v = std::make_shared<VariableNode>(VariableNode("v", 3));
        auto assignNode3 = std::make_shared<AssignmentNode>(ANode_z, ANode_v, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(assignNode2);
        childrenProc2.push_back(assignNode3);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{1, "x"}, {2, "x"}, {3, "z"}};
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("MODIFIES extracted correctly - WHILE statements") {
        /*
            procedure proc1 {
                01 while ((c < d) && (e > f)) {
                02     g = h;
                   }
            }
            procedure proc2 {
                03 while ((c < d) && (e > f)) {
                04     read a;
                   }
            }
        */

        // 01 while ((c < d) && (e > f)) {
        // 02     g = h;
        //    }
        std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 1));
        std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 1));
        std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 1));
        std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 1));
        std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 2));
        std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 2));

        std::vector<std::shared_ptr<StatementNode>> WChildren = {};

        auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 1);
        auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 1);
        auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 1);

        auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 2);
        WChildren.push_back(Wassign);

        auto WstmtList = std::make_shared<StatementListNode>(WChildren);
        auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(whileNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 03 while ((c < d) && (e > f)) {
        // 04     read a;
        //    }
        std::shared_ptr<VariableNode> WNode_c2 = std::make_shared<VariableNode>(VariableNode("c", 3));
        std::shared_ptr<VariableNode> WNode_d2 = std::make_shared<VariableNode>(VariableNode("d", 3));
        std::shared_ptr<VariableNode> WNode_e2 = std::make_shared<VariableNode>(VariableNode("e", 3));
        std::shared_ptr<VariableNode> WNode_f2 = std::make_shared<VariableNode>(VariableNode("f", 3));
        std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 4));

        std::vector<std::shared_ptr<StatementNode>> WChildren2 = {};

        auto WlessThan2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c2, WNode_d2, 3);
        auto WmoreThan2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e2, WNode_f2, 3);
        auto WandandNode2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan2, WmoreThan2, 3);

        auto readNode = std::make_shared<ReadNode>(RNode_a, 4);
        WChildren2.push_back(readNode);

        auto WstmtList2 = std::make_shared<StatementListNode>(WChildren2);
        auto whileNode2 = std::make_shared<WhileNode>(WandandNode2, WstmtList2, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(whileNode2);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{2, "g"}, {1, "g"}, {4, "a"}, {3, "a"}};
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("MODIFIES extracted correctly - IF statements") {
        /*
            procedure proc1 {
                01 if (i < j) then {
                02     x = y + 2;
                   } else {
                03     print b;
                   }
            }
            procedure proc2 {
                04 if (i > j) then {
                05     read a;
                   } else {
                06     y = v;
                   }
            }
        */

        // 01 if (i < j) then {
        // 02     x = y + 2;
        //    } else {
        // 03     print b;
        //    }
        std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 1));
        std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 1));
        std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 2));
        std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 2));
        std::shared_ptr<ExpressionNode> INode_ADD =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 2));
        std::shared_ptr<VariableNode> INode_b = std::make_shared<VariableNode>(VariableNode("b", 3));

        auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 1);

        std::vector<std::shared_ptr<StatementNode>> IChildren = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren = {};

        auto Iassign = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 2);
        IChildren.push_back(Iassign);

        auto printNode = std::make_shared<PrintNode>(INode_b, 3);
        EChildren.push_back(printNode);

        auto IstmtList = std::make_shared<StatementListNode>(IChildren);
        auto EstmtList = std::make_shared<StatementListNode>(EChildren);
        auto ifNode = std::make_shared<IfNode>(IlessThan, IstmtList, EstmtList, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(ifNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 04 if (i > j) then {
        // 05     read a;
        //    } else {
        // 06     y = v;
        //    }
        std::shared_ptr<VariableNode> INode_i2 = std::make_shared<VariableNode>(VariableNode("i", 4));
        std::shared_ptr<VariableNode> INode_j2 = std::make_shared<VariableNode>(VariableNode("j", 4));
        std::shared_ptr<VariableNode> INode_a = std::make_shared<VariableNode>(VariableNode("a", 5));
        std::shared_ptr<VariableNode> INode_y2 = std::make_shared<VariableNode>(VariableNode("y", 6));
        std::shared_ptr<VariableNode> INode_v = std::make_shared<VariableNode>(VariableNode("v", 6));

        auto IgreaterThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, INode_i2, INode_j2, 4);

        std::vector<std::shared_ptr<StatementNode>> IChildren2 = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren2 = {};

        auto readNode = std::make_shared<ReadNode>(INode_a, 5);
        IChildren2.push_back(readNode);

        auto assignNode = std::make_shared<AssignmentNode>(INode_y2, INode_v, 6);
        EChildren2.push_back(assignNode);

        auto IstmtList2 = std::make_shared<StatementListNode>(IChildren2);
        auto EstmtList2 = std::make_shared<StatementListNode>(EChildren2);
        auto ifNode2 = std::make_shared<IfNode>(IgreaterThan, IstmtList2, EstmtList2, 4);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(ifNode2);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        /*
            procedure proc1 {
                01 if (i < j) then {
                02     x = y + 2;
                   } else {
                03     print b;
                   }
            }
            procedure proc2 {
                04 if (i > j) then {
                05     read a;
                   } else {
                06     y = v;
                   }
            }
        */
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{2, "x"}, {1, "x"}, {5, "a"},
                                                                             {4, "a"}, {6, "y"}, {4, "y"}};
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("MODIFIES extracted correctly - CALL statements") {
        /*
            procedure proc1 {
                01 read z;
            }
            procedure proc2 {
                02 read y;
                03 call proc1;
            }
        */
        // 01 read z;
        std::shared_ptr<VariableNode> RNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto readNode = std::make_shared<ReadNode>(RNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(readNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 read y;
        std::shared_ptr<VariableNode> RNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto readNode1 = std::make_shared<ReadNode>(RNode_y, 2);

        // 03 call proc1
        auto callNode = std::make_shared<CallNode>("proc1", 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(readNode1);
        childrenProc2.push_back(callNode);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedModifies = {{1, "z"}, {2, "y"}, {3, "z"}};
        REQUIRE(expectedModifies == designExtractor.getModifies());
    }

    SECTION("MODIFIES extracted correctly - PROCEDURES") {
        /*
            procedure proc1 {
                01 read z;
            }
            procedure proc2 {
                02 read y;
                03 call proc1;
            }
        */
        // 01 read z;
        std::shared_ptr<VariableNode> RNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto readNode = std::make_shared<ReadNode>(RNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(readNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 read y;
        std::shared_ptr<VariableNode> RNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto readNode1 = std::make_shared<ReadNode>(RNode_y, 2);

        // 03 call proc1
        auto callNode = std::make_shared<CallNode>("proc1", 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(readNode1);
        childrenProc2.push_back(callNode);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<Procedure, Variable>> expectedProcedureModifies = {
            {"proc1", "z"}, {"proc2", "y"}, {"proc2", "z"}};
        REQUIRE(expectedProcedureModifies == designExtractor.getProcedureModifies());
    }
}

TEST_CASE("Uses Extractor Tests") {
    SECTION("USES extracted correctly - PRINT statements") {
        /*
            procedure proc1 {
                01 print z;
            }
            procedure proc2 {
                02 print y;
            }
        */
        // 01 print z;
        std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto printNode = std::make_shared<PrintNode>(PNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(printNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 print y;
        std::shared_ptr<VariableNode> PNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto printNode1 = std::make_shared<PrintNode>(PNode_y, 2);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(printNode1);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {{1, "z"}, {2, "y"}};
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("USES extracted correctly - ASSIGN statements") {
        /*
            procedure proc1 {
                01 x = 2 * (5 - y);
            }
            procedure proc2 {
                02 x = 4;
                03 z = v;
            }
        */
        // 01 x = 2 * (5 - y);
        std::shared_ptr<VariableNode> ANode_x = std::make_shared<VariableNode>(VariableNode("x", 1));
        std::shared_ptr<VariableNode> ANode_y = std::make_shared<VariableNode>(VariableNode("y", 1));
        std::shared_ptr<ConstantNode> ANode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 1));
        std::shared_ptr<ConstantNode> ANode_5 = std::make_shared<ConstantNode>(ConstantNode("5", 1));
        std::shared_ptr<ExpressionNode> ANode_SUB =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::SUB, ANode_5, ANode_y, 1));
        std::shared_ptr<ExpressionNode> ANode_MUL =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::MUL, ANode_2, ANode_SUB, 1));
        auto assignNode = std::make_shared<AssignmentNode>(ANode_x, ANode_MUL, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(assignNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 x = 4;
        std::shared_ptr<VariableNode> ANode_x1 = std::make_shared<VariableNode>(VariableNode("x", 2));
        std::shared_ptr<ConstantNode> ANode_4 = std::make_shared<ConstantNode>(ConstantNode("4", 2));
        auto assignNode2 = std::make_shared<AssignmentNode>(ANode_x1, ANode_4, 2);

        // 03 z = v;
        std::shared_ptr<VariableNode> ANode_z = std::make_shared<VariableNode>(VariableNode("z", 3));
        std::shared_ptr<VariableNode> ANode_v = std::make_shared<VariableNode>(VariableNode("v", 3));
        auto assignNode3 = std::make_shared<AssignmentNode>(ANode_z, ANode_v, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(assignNode2);
        childrenProc2.push_back(assignNode3);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        /*
            procedure proc1 {
                01 x = 2 * (5 - y);
            }
            procedure proc2 {
                02 x = 4;
                03 z = v;
            }
        */

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {{1, "y"}, {3, "v"}};
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("USES extracted correctly - WHILE statements") {
        /*
            procedure proc1 {
                01 while ((c < d) && (e > f)) {
                02     g = h;
                   }
            }
            procedure proc2 {
                03 while ((c < d) && (e > f)) {
                04     print a;
                   }
            }
        */

        // 01 while ((c < d) && (e > f)) {
        // 02     g = h;
        //    }
        std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 1));
        std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 1));
        std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 1));
        std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 1));
        std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 2));
        std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 2));

        std::vector<std::shared_ptr<StatementNode>> WChildren = {};

        auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 1);
        auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 1);
        auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 1);

        auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 2);
        WChildren.push_back(Wassign);

        auto WstmtList = std::make_shared<StatementListNode>(WChildren);
        auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(whileNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 03 while ((c < d) && (e > f)) {
        // 04     print a;
        //    }
        std::shared_ptr<VariableNode> WNode_c2 = std::make_shared<VariableNode>(VariableNode("c", 3));
        std::shared_ptr<VariableNode> WNode_d2 = std::make_shared<VariableNode>(VariableNode("d", 3));
        std::shared_ptr<VariableNode> WNode_e2 = std::make_shared<VariableNode>(VariableNode("e", 3));
        std::shared_ptr<VariableNode> WNode_f2 = std::make_shared<VariableNode>(VariableNode("f", 3));
        std::shared_ptr<VariableNode> PNode_a = std::make_shared<VariableNode>(VariableNode("a", 4));

        std::vector<std::shared_ptr<StatementNode>> WChildren2 = {};

        auto WlessThan2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c2, WNode_d2, 3);
        auto WmoreThan2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e2, WNode_f2, 3);
        auto WandandNode2 = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan2, WmoreThan2, 3);

        auto printNode = std::make_shared<PrintNode>(PNode_a, 4);
        WChildren2.push_back(printNode);

        auto WstmtList2 = std::make_shared<StatementListNode>(WChildren2);
        auto whileNode2 = std::make_shared<WhileNode>(WandandNode2, WstmtList2, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(whileNode2);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {{1, "c"}, {1, "d"}, {1, "e"}, {1, "f"},
                                                                         {2, "h"}, {1, "h"}, {3, "c"}, {3, "d"},
                                                                         {3, "e"}, {3, "f"}, {4, "a"}, {3, "a"}};
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("USES extracted correctly - IF statements") {
        /*
            procedure proc1 {
                01 if (i < j) then {
                02     x = y + 2;
                   } else {
                03     print b;
                   }
            }
            procedure proc2 {
                04 if (i > j) then {
                05     read a;
                   } else {
                06     y = v;
                   }
            }
        */

        // 01 if (i < j) then {
        // 02     x = y + 2;
        //    } else {
        // 03     print b;
        //    }
        std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 1));
        std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 1));
        std::shared_ptr<VariableNode> INode_x = std::make_shared<VariableNode>(VariableNode("x", 2));
        std::shared_ptr<VariableNode> INode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        std::shared_ptr<ConstantNode> INode_2 = std::make_shared<ConstantNode>(ConstantNode("2", 2));
        std::shared_ptr<ExpressionNode> INode_ADD =
            std::make_shared<ExpressionNode>(ExpressionNode(LEXICAL_TOKEN_TYPE::ADD, INode_y, INode_2, 2));
        std::shared_ptr<VariableNode> INode_b = std::make_shared<VariableNode>(VariableNode("b", 3));

        auto IlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 1);

        std::vector<std::shared_ptr<StatementNode>> IChildren = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren = {};

        auto Iassign = std::make_shared<AssignmentNode>(INode_x, INode_ADD, 2);
        IChildren.push_back(Iassign);

        auto printNode = std::make_shared<PrintNode>(INode_b, 3);
        EChildren.push_back(printNode);

        auto IstmtList = std::make_shared<StatementListNode>(IChildren);
        auto EstmtList = std::make_shared<StatementListNode>(EChildren);
        auto ifNode = std::make_shared<IfNode>(IlessThan, IstmtList, EstmtList, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(ifNode);

        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 04 if (i > j) then {
        // 05     read a;
        //    } else {
        // 06     y = v;
        //    }
        std::shared_ptr<VariableNode> INode_i2 = std::make_shared<VariableNode>(VariableNode("i", 4));
        std::shared_ptr<VariableNode> INode_j2 = std::make_shared<VariableNode>(VariableNode("j", 4));
        std::shared_ptr<VariableNode> INode_a = std::make_shared<VariableNode>(VariableNode("a", 5));
        std::shared_ptr<VariableNode> INode_y2 = std::make_shared<VariableNode>(VariableNode("y", 6));
        std::shared_ptr<VariableNode> INode_v = std::make_shared<VariableNode>(VariableNode("v", 6));

        auto IgreaterThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, INode_i2, INode_j2, 4);

        std::vector<std::shared_ptr<StatementNode>> IChildren2 = {};
        std::vector<std::shared_ptr<StatementNode>> EChildren2 = {};

        auto readNode = std::make_shared<ReadNode>(INode_a, 5);
        IChildren2.push_back(readNode);

        auto assignNode = std::make_shared<AssignmentNode>(INode_y2, INode_v, 6);
        EChildren2.push_back(assignNode);

        auto IstmtList2 = std::make_shared<StatementListNode>(IChildren2);
        auto EstmtList2 = std::make_shared<StatementListNode>(EChildren2);
        auto ifNode2 = std::make_shared<IfNode>(IgreaterThan, IstmtList2, EstmtList2, 4);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(ifNode2);

        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);

        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {
            {1, "i"}, {1, "j"}, {2, "y"}, {1, "y"}, {3, "b"}, {1, "b"}, {4, "i"}, {4, "j"}, {6, "v"}, {4, "v"}};
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("USES extracted correctly - CALL statements") {
        /*
            procedure proc1 {
                01 print z;
            }
            procedure proc2 {
                02 print y;
                03 call proc1;
            }
        */
        // 01 print z;
        std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto printNode = std::make_shared<PrintNode>(PNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(printNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 print y;
        std::shared_ptr<VariableNode> PNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto printNode1 = std::make_shared<PrintNode>(PNode_y, 2);

        // 03 call proc1
        auto callNode = std::make_shared<CallNode>("proc1", 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(printNode1);
        childrenProc2.push_back(callNode);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, Variable>> expectedUses = {{1, "z"}, {2, "y"}, {3, "z"}};
        REQUIRE(expectedUses == designExtractor.getUses());
    }

    SECTION("USES extracted correctly - PROCEDURES") {
        /*
            procedure proc1 {
                01 print z;
            }
            procedure proc2 {
                02 print y;
                03 call proc1;
            }
        */
        // 01 print z;
        std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 1));
        auto printNode = std::make_shared<PrintNode>(PNode_z, 1);

        std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
        childrenProc1.push_back(printNode);
        auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
        auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

        // 02 print y;
        std::shared_ptr<VariableNode> PNode_y = std::make_shared<VariableNode>(VariableNode("y", 2));
        auto printNode1 = std::make_shared<PrintNode>(PNode_y, 2);

        // 03 call proc1
        auto callNode = std::make_shared<CallNode>("proc1", 3);

        std::vector<std::shared_ptr<StatementNode>> childrenProc2 = {};
        childrenProc2.push_back(printNode1);
        childrenProc2.push_back(callNode);
        auto stmtListProc2 = std::make_shared<StatementListNode>(childrenProc2);
        auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

        // Create program
        std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
        childrenProg.push_back(Proc1Node);
        childrenProg.push_back(Proc2Node);
        auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<Procedure, Variable>> expectedProcedureUses = {
            {"proc1", "z"}, {"proc2", "y"}, {"proc2", "z"}};
        REQUIRE(expectedProcedureUses == designExtractor.getProcedureUses());
    }
}

TEST_CASE("Calls Extractor Tests") {
    SECTION("CALLS extracted correctly") {
        /*
            procedure sad {
                01 call happy;
            }
            procedure happy {
                02 print a;
                03 read b;
            }
        */
        auto callNode = std::make_shared<CallNode>("happy", 1);
        std::vector<std::shared_ptr<StatementNode>> childrenCall = {};
        childrenCall.push_back(callNode);
        auto stmtListProcCall = std::make_shared<StatementListNode>(childrenCall);
        auto ProcNodeCall = std::make_shared<ProcedureNode>("sad", stmtListProcCall);

        auto variableCall = std::make_shared<VariableNode>("a", 2);
        auto printCall = std::make_shared<PrintNode>(variableCall, 2);

        auto variableCall2 = std::make_shared<VariableNode>("b", 3);
        auto readCall = std::make_shared<ReadNode>(variableCall2, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenCall2 = {};
        childrenCall2.push_back(printCall);
        childrenCall2.push_back(readCall);
        auto stmtListProcCall2 = std::make_shared<StatementListNode>(childrenCall2);
        auto ProcNodeCall2 = std::make_shared<ProcedureNode>("happy", stmtListProcCall2);

        std::vector<std::shared_ptr<ProcedureNode>> childrenProgCall = {};

        childrenProgCall.push_back(ProcNodeCall);
        childrenProgCall.push_back(ProcNodeCall2);
        auto ProgNodeCall = std::make_shared<ProgramNode>(childrenProgCall);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNodeCall);
        std::unordered_set<std::pair<Procedure, Procedure>> expectedCalls = {{"sad", "happy"}};
        REQUIRE(expectedCalls == designExtractor.getCalls());
    }

    SECTION("CALL statements extracted correctly") {
        /*
            procedure sad {
                01 call happy;
            }
            procedure happy {
                02 print a;
                03 read b;
            }
        */
        auto callNode = std::make_shared<CallNode>("happy", 1);
        std::vector<std::shared_ptr<StatementNode>> childrenCall = {};
        childrenCall.push_back(callNode);
        auto stmtListProcCall = std::make_shared<StatementListNode>(childrenCall);
        auto ProcNodeCall = std::make_shared<ProcedureNode>("sad", stmtListProcCall);

        auto variableCall = std::make_shared<VariableNode>("a", 2);
        auto printCall = std::make_shared<PrintNode>(variableCall, 2);

        auto variableCall2 = std::make_shared<VariableNode>("b", 3);
        auto readCall = std::make_shared<ReadNode>(variableCall2, 3);

        std::vector<std::shared_ptr<StatementNode>> childrenCall2 = {};
        childrenCall2.push_back(printCall);
        childrenCall2.push_back(readCall);
        auto stmtListProcCall2 = std::make_shared<StatementListNode>(childrenCall2);
        auto ProcNodeCall2 = std::make_shared<ProcedureNode>("happy", stmtListProcCall2);

        std::vector<std::shared_ptr<ProcedureNode>> childrenProgCall = {};

        childrenProgCall.push_back(ProcNodeCall);
        childrenProgCall.push_back(ProcNodeCall2);
        auto ProgNodeCall = std::make_shared<ProgramNode>(childrenProgCall);

        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNodeCall);
        std::unordered_set<std::pair<Procedure, StmtNum>> expectedCallStmts = {{"happy", 1}};
        REQUIRE(expectedCallStmts == designExtractor.getCallStmts());
    }

    SECTION("CALLS should not work - Single PROCEDURE") {
        // might not need this i think is already tested in semantic vali
        /*
            procedure sad {
                01 call happy;
            }
        */
        auto callNode = std::make_shared<CallNode>("happy", 1);
        std::vector<std::shared_ptr<StatementNode>> childrenCall = {};
        childrenCall.push_back(callNode);
        auto stmtListProcCall = std::make_shared<StatementListNode>(childrenCall);
        auto ProcNodeCall = std::make_shared<ProcedureNode>("sad", stmtListProcCall);

        std::vector<std::shared_ptr<ProcedureNode>> childrenProgCall = {};

        childrenProgCall.push_back(ProcNodeCall);
        auto ProgNodeCall = std::make_shared<ProgramNode>(childrenProgCall);

        DesignExtractor designExtractor = DesignExtractor();
        REQUIRE_THROWS(designExtractor.extract(ProgNodeCall));
    }
}

// Pattern Extractor Tests -- this will be refactored once pattern implementation is done
TEST_CASE("Pattern Extractor Tests") {
    /*
        procedure proc1 {
            01 read a;
            02 while ((c < d) && (e > f)) {
            03     g = h;
               }
        }
        procedure proc2 {
            04 print z;
            05 if (!(i < j)) then {
            06     if ((a == b) || (c == d)) {
            07          e = f;
            } else {
            08      g = h;
            }
               } else {
            09     k = l;
               }
        }

        procedure proc3 {
            10 while ( i <  0) {
                11 if (i < j) then {
                12 a = c;
                } else {
                13 b = d;
                }
            }
        }
    */

    // 01 read a;
    std::shared_ptr<VariableNode> RNode_a = std::make_shared<VariableNode>(VariableNode("a", 1));
    auto readNode = std::make_shared<ReadNode>(RNode_a, 1);

    // 02 while ((c < d) && (e > f)) {
    // 03     g = h;
    //    }
    std::shared_ptr<VariableNode> WNode_c = std::make_shared<VariableNode>(VariableNode("c", 2));
    std::shared_ptr<VariableNode> WNode_d = std::make_shared<VariableNode>(VariableNode("d", 2));
    std::shared_ptr<VariableNode> WNode_e = std::make_shared<VariableNode>(VariableNode("e", 2));
    std::shared_ptr<VariableNode> WNode_f = std::make_shared<VariableNode>(VariableNode("f", 2));
    std::shared_ptr<VariableNode> WNode_g = std::make_shared<VariableNode>(VariableNode("g", 3));
    std::shared_ptr<VariableNode> WNode_h = std::make_shared<VariableNode>(VariableNode("h", 3));

    std::vector<std::shared_ptr<StatementNode>> WChildren = {};

    auto WlessThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, WNode_c, WNode_d, 2);
    auto WmoreThan = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::GREATER_THAN, WNode_e, WNode_f, 2);
    auto WandandNode = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::ANDAND, WlessThan, WmoreThan, 2);

    auto Wassign = std::make_shared<AssignmentNode>(WNode_g, WNode_h, 3);
    WChildren.push_back(Wassign);

    auto WstmtList = std::make_shared<StatementListNode>(WChildren);
    auto whileNode = std::make_shared<WhileNode>(WandandNode, WstmtList, 2);

    std::vector<std::shared_ptr<StatementNode>> childrenProc1 = {};
    childrenProc1.push_back(readNode);
    childrenProc1.push_back(whileNode);

    auto stmtListProc1 = std::make_shared<StatementListNode>(childrenProc1);
    auto Proc1Node = std::make_shared<ProcedureNode>("proc1", stmtListProc1);

    // 04 print z;
    std::shared_ptr<VariableNode> PNode_z = std::make_shared<VariableNode>(VariableNode("z", 4));
    auto printNode = std::make_shared<PrintNode>(PNode_z, 4);

    // 05 if (!(i < j)) then {
    // 06     if ((a == b) || (c == d)) {
    // 07          e = f;
    // } else {
    // 08      g = h;
    // }
    //    } else {
    // 09     k = l;
    //    }
    std::shared_ptr<VariableNode> INode_i = std::make_shared<VariableNode>(VariableNode("i", 5));
    std::shared_ptr<VariableNode> INode_j = std::make_shared<VariableNode>(VariableNode("j", 5));
    std::shared_ptr<VariableNode> IINode_a = std::make_shared<VariableNode>(VariableNode("a", 6));
    std::shared_ptr<VariableNode> IINode_b = std::make_shared<VariableNode>(VariableNode("b", 6));
    std::shared_ptr<VariableNode> IINode_c = std::make_shared<VariableNode>(VariableNode("c", 6));
    std::shared_ptr<VariableNode> IINode_d = std::make_shared<VariableNode>(VariableNode("d", 6));
    std::shared_ptr<VariableNode> IINode_e = std::make_shared<VariableNode>(VariableNode("e", 7));
    std::shared_ptr<VariableNode> IINode_f = std::make_shared<VariableNode>(VariableNode("f", 7));
    std::shared_ptr<VariableNode> IINode_g = std::make_shared<VariableNode>(VariableNode("g", 8));
    std::shared_ptr<VariableNode> IINode_h = std::make_shared<VariableNode>(VariableNode("h", 8));
    std::shared_ptr<VariableNode> IINode_k = std::make_shared<VariableNode>(VariableNode("k", 9));
    std::shared_ptr<VariableNode> IINode_l = std::make_shared<VariableNode>(VariableNode("l", 9));

    auto line5Condition = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, INode_i, INode_j, 5);
    auto line6ConditionLHS = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::EQUAL_CHECK, IINode_a, IINode_b, 6);
    auto line6ConditionRHS = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::EQUAL_CHECK, IINode_c, IINode_d, 6);
    auto line6Condition =
        std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::OR, line6ConditionLHS, line6ConditionRHS, 6);

    auto line7Assign = std::make_shared<AssignmentNode>(IINode_e, IINode_f, 7);
    auto line8Assign = std::make_shared<AssignmentNode>(IINode_g, IINode_h, 8);
    auto innerThenLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({line7Assign}));
    auto innerElseLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({line8Assign}));
    auto innerIf = std::make_shared<IfNode>(line6Condition, innerThenLst, innerElseLst, 6);

    auto line9Assign = std::make_shared<AssignmentNode>(IINode_k, IINode_l, 9);
    auto outerThenLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({innerIf}));
    auto outerElseLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({line9Assign}));

    auto outerIf = std::make_shared<IfNode>(line5Condition, outerThenLst, outerElseLst, 5);

    auto stmtListProc2 =
        std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({printNode, outerIf}));
    auto Proc2Node = std::make_shared<ProcedureNode>("proc2", stmtListProc2);

    // 10 while ( i <  0) {
    //     11 if (i < j) then {
    //     12 a = c;
    //     } else {
    //     13 b = d;
    //     }
    // }
    auto wNode_i = std::make_shared<VariableNode>(VariableNode("i", 10));
    auto wNodeI_i = std::make_shared<VariableNode>(VariableNode("i", 11));
    auto wNodeI_j = std::make_shared<VariableNode>(VariableNode("j", 11));
    auto wNodeI_a = std::make_shared<VariableNode>(VariableNode("a", 12));
    auto wNodeI_c = std::make_shared<VariableNode>(VariableNode("c", 12));
    auto wNodeI_b = std::make_shared<VariableNode>(VariableNode("b", 13));
    auto wNodeI_d = std::make_shared<VariableNode>(VariableNode("d", 13));
    auto wNode_0 = std::make_shared<ConstantNode>(ConstantNode("0", 10));

    auto line10Condition = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, wNode_i, wNode_0, 10);
    auto line11Condition = std::make_shared<ExpressionNode>(LEXICAL_TOKEN_TYPE::LESS_THAN, wNodeI_i, wNodeI_j, 11);
    auto line12Assn = std::make_shared<AssignmentNode>(wNodeI_a, wNodeI_c, 12);
    auto line13Assn = std::make_shared<AssignmentNode>(wNodeI_b, wNodeI_d, 13);

    auto thenLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({line12Assn}));
    auto elseLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({line13Assn}));
    auto ifBlock = std::make_shared<IfNode>(line11Condition, thenLst, elseLst, 11);
    auto whileLst = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({ifBlock}));
    auto whileBlock = std::make_shared<WhileNode>(line10Condition, whileLst, 10);

    auto stmtListProc3 = std::make_shared<StatementListNode>(std::vector<std::shared_ptr<StatementNode>>({whileBlock}));
    auto Proc3Node = std::make_shared<ProcedureNode>("proc3", stmtListProc3);

    // Create program
    std::vector<std::shared_ptr<ProcedureNode>> childrenProg = {};
    childrenProg.push_back(Proc1Node);
    childrenProg.push_back(Proc2Node);
    childrenProg.push_back(Proc3Node);

    auto ProgNode = std::make_shared<ProgramNode>(childrenProg);

    SECTION("If Pattern Extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, std::string>> expectedIfVariables = {
            {5, "i"}, {5, "j"}, {6, "a"}, {6, "b"}, {6, "c"}, {6, "d"}, {11, "i"}, {11, "j"}};
        REQUIRE(expectedIfVariables == designExtractor.getIfPattern());
    }

    SECTION("While Pattern Extracted correctly") {
        DesignExtractor designExtractor = DesignExtractor();
        designExtractor.extract(ProgNode);
        std::unordered_set<std::pair<StmtNum, std::string>> expectedWhileVariables = {
            {2, "c"}, {2, "d"}, {2, "e"}, {2, "f"}, {10, "i"}};
        REQUIRE(expectedWhileVariables == designExtractor.getWhilePattern());
    }

    // SECTION("Pattern extracted correctly") {
    //     DesignExtractor designExtractor = DesignExtractor();
    //     designExtractor.extract(ProgNode);
    //     std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {
    //         {1, {"x", "y"}}, {5, {"g", "h"}}, {7, {"i", "j"}}, {8, {"a", "b"}}, {9, {"x", "1"}}};
    //     REQUIRE(expectedPattern == designExtractor.getPattern());
    // }

    // SECTION("Single line procedure has pattern extracted correctly") {
    //     DesignExtractor designExtractor = DesignExtractor();
    //     designExtractor.extract(ProgNode2);
    //     std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {};
    //     REQUIRE(expectedPattern == designExtractor.getPattern());
    // }

    // SECTION("Multiple procedures pattern extracted correctly") {
    //     DesignExtractor designExtractor = DesignExtractor();
    //     designExtractor.extract(ProgNode3);
    //     std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> expectedPattern = {
    //         {1, {"x", "y"}}, {5, {"g", "h"}}, {7, {"i", "j"}}, {8, {"a", "b"}}, {9, {"x", "1"}}};
    //     REQUIRE(expectedPattern == designExtractor.getPattern());
    // }
}

// Next Extractor Tests
TEST_CASE("Next Extractor Tests") {}
