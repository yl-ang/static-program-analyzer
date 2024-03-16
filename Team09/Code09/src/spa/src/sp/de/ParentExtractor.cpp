#include "ParentExtractor.h"

#include "NodeDeclarations.h"

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
void ParentExtractor::visitCall(CallNode* node) {}

void ParentExtractor::visitWhile(WhileNode* node) {
    int parentStmtNum = node->statementNumber;
    std::vector<int> childrenStmtNums = node->whileStmtList->getStmtsStmtNum();
    for (int i = 0; i < childrenStmtNums.size(); ++i) {
        this->parent.insert({parentStmtNum, childrenStmtNums[i]});
    }
}

void ParentExtractor::visitIf(IfNode* node) {
    int parentStmtNum = node->statementNumber;
    std::vector<int> thenStmtNums = node->thenStmtList->getStmtsStmtNum();
    std::vector<int> elseStmtNums = node->elseStmtList->getStmtsStmtNum();
    for (int i = 0; i < thenStmtNums.size(); ++i) {
        this->parent.insert({parentStmtNum, thenStmtNums[i]});
    }
    for (int i = 0; i < elseStmtNums.size(); ++i) {
        this->parent.insert({parentStmtNum, elseStmtNums[i]});
    }
}

std::unordered_set<std::pair<StmtNum, StmtNum>> ParentExtractor::getParent() {
    return this->parent;
}
