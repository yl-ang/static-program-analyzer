#include "ParentExtractor.h"

#include <vector>

void ParentExtractor::visitWhile(WhileNode* node) {
    int parentStmtNum = node->getStmtNum();
    std::vector<int> childrenStmtNums = node->getStmtLstNode()->getStmtsStmtNum();
    for (int i = 0; i < childrenStmtNums.size(); ++i) {
        this->follows.insert({parentStmtNum, childrenStmtNums[i]});
    }
}

void ParentExtractor::visitIf(IfNode* node) {
    // int parentStmtNum = node->getStmtNum();
    // std::vector<int> childrenStmtNums = node->getStmtLstNode()->getStmtsStmtNum();
    // for (int i = 0; i < childrenStmtNums.size(); ++i) {
    //     this->follows.insert({parentStmtNum, childrenStmtNums[i]});
    // }
}

void ParentExtractor::visitProgram(ProgramNode* node) {}
void ParentExtractor::visitStmtLst(StatementListNode* node) {}
void ParentExtractor::visitExpression(ExpressionNode* node) {}
void ParentExtractor::visitFactor(FactorNode* node) {}
void ParentExtractor::visitTerm(TermNode* node) {}
void ParentExtractor::visitProcedure(ProcedureNode* node) {}
void ParentExtractor::visitRead(ReadNode* node) {}
void ParentExtractor::visitPrint(PrintNode* node) {}
void ParentExtractor::visitAssign(AssignmentNode* node) {}
void ParentExtractor::visitVariable(VariableNode* node) {}
void ParentExtractor::visitConstant(ConstantNode* node) {}

std::unordered_set<std::pair<StmtNum, StmtNum>> ParentExtractor::getParent() {
    return this->parent;
}
