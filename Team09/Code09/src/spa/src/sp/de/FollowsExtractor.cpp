#include "FollowsExtractor.h"

#include <vector>

void FollowsExtractor::visitProgram(ProgramNode* node) {}
void FollowsExtractor::visitExpression(ExpressionNode* node) {}
void FollowsExtractor::visitFactor(FactorNode* node) {}
void FollowsExtractor::visitTerm(TermNode* node) {}
void FollowsExtractor::visitProcedure(ProcedureNode* node) {}
void FollowsExtractor::visitRead(ReadNode* node) {}
void FollowsExtractor::visitPrint(PrintNode* node) {}
void FollowsExtractor::visitWhile(WhileNode* node) {}
void FollowsExtractor::visitIf(IfNode* node) {}
void FollowsExtractor::visitAssign(AssignmentNode* node) {}
void FollowsExtractor::visitVariable(VariableNode* node) {}
void FollowsExtractor::visitConstant(ConstantNode* node) {}

void FollowsExtractor::visitStmtLst(StatementListNode* node) {
    std::vector<int> stmtNumbers = node->getStmtsStmtNum();
    for (int i = 0; i < stmtNumbers.size() - 1; ++i) {
        this->follows.insert({stmtNumbers[i], stmtNumbers[i + 1]});
    }
}

std::unordered_set<std::pair<StmtNum, StmtNum>> FollowsExtractor::getFollows() {
    return this->follows;
}
