#include "PatternExtractor.h"

void PatternExtractor::visitPrint(PrintNode* node) {}
void PatternExtractor::visitWhile(WhileNode* node) {}
void PatternExtractor::visitIf(IfNode* node) {}
void PatternExtractor::visitProgram(ProgramNode* node) {}
void PatternExtractor::visitStmtLst(StatementListNode* node) {}
void PatternExtractor::visitExpression(ExpressionNode* node) {}
void PatternExtractor::visitFactor(FactorNode* node) {}
void PatternExtractor::visitTerm(TermNode* node) {}
void PatternExtractor::visitProcedure(ProcedureNode* node) {}
void PatternExtractor::visitRead(ReadNode* node) {}
void PatternExtractor::visitVariable(VariableNode* node) {}
void PatternExtractor::visitConstant(ConstantNode* node) {}

void PatternExtractor::visitAssign(AssignmentNode* node) {
    int stmtNum = node->getStmtNum();
    std::string lhs = node->getVar();
    std::vector<std::string> rhsVars = node->getExpr()->getVars();
    std::vector<std::string> rhsConsts = node->getExpr()->getConsts();
    for (int i = 0; i < rhsVars.size(); ++i) {
        this->pattern.insert({stmtNum, {lhs, rhsVars[i]}});
    }
    for (int i = 0; i < rhsConsts.size(); ++i) {
        this->pattern.insert({stmtNum, {lhs, rhsConsts[i]}});
    }
}

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> PatternExtractor::getPattern() {
    return this->pattern;
}
