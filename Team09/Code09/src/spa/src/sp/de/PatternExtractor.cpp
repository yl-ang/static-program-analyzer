#include "PatternExtractor.h"

void PatternExtractor::visitAssign(AssignmentNode* node) {}

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

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> PatternExtractor::getPattern() {
    return this->pattern;
}
