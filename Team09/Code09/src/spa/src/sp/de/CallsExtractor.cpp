#include "CallsExtractor.h"

#include "NodeDeclarations.h"

void CallsExtractor::visitProgram(ProgramNode* node) {}
void CallsExtractor::visitStmtLst(StatementListNode* node) {}
void CallsExtractor::visitExpression(ExpressionNode* node) {}
void CallsExtractor::visitFactor(FactorNode* node) {}
void CallsExtractor::visitTerm(TermNode* node) {}
void CallsExtractor::visitRead(ReadNode* node) {}
void CallsExtractor::visitPrint(PrintNode* node) {}
void CallsExtractor::visitAssign(AssignmentNode* node) {}
void CallsExtractor::visitVariable(VariableNode* node) {}
void CallsExtractor::visitConstant(ConstantNode* node) {}
void CallsExtractor::visitWhile(WhileNode* node) {}
void CallsExtractor::visitIf(IfNode* node) {}

void CallsExtractor::visitProcedure(ProcedureNode* node) {
    this->currentProc = node->name;
}

void CallsExtractor::visitCall(CallNode* node) {
    this->calls.insert({this->currentProc, node->getCalledProcedure()});
}

std::unordered_set<std::pair<Procedure, Procedure>> CallsExtractor::getCalls() {
    return this->calls;
}
