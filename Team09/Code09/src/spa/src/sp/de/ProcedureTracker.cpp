#include "ProcedureTracker.h"

#include <vector>

#include "NodeDeclarations.h"

void ProcedureTracker::visitProgram(ProgramNode* node) {}
void ProcedureTracker::visitExpression(ExpressionNode* node) {}
void ProcedureTracker::visitFactor(FactorNode* node) {}
void ProcedureTracker::visitTerm(TermNode* node) {}
void ProcedureTracker::visitRead(ReadNode* node) {}
void ProcedureTracker::visitPrint(PrintNode* node) {}
void ProcedureTracker::visitWhile(WhileNode* node) {}
void ProcedureTracker::visitIf(IfNode* node) {}
void ProcedureTracker::visitAssign(AssignmentNode* node) {}
void ProcedureTracker::visitVariable(VariableNode* node) {}
void ProcedureTracker::visitConstant(ConstantNode* node) {}
void ProcedureTracker::visitCall(CallNode* node) {}
void ProcedureTracker::visitStmtLst(StatementListNode* node) {}

void ProcedureTracker::visitProcedure(ProcedureNode* node) {
    std::string procName = node->getValue();
    this->procedures.insert({procName, node});
}
