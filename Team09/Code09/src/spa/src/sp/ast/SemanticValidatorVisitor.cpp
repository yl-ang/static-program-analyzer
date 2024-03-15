#include "SemanticValidatorVisitor.h"

#include "sp/de/NodeDeclarations.h"
#include "sp/exceptions/semantic/NoRecursiveCallsAllowedError.h"
#include "sp/exceptions/semantic/NotCallingProcedureError.h"

void SemanticValidatorVisitor::visitProgram(ProgramNode* node) {}
void SemanticValidatorVisitor::visitStmtLst(StatementListNode* node) {}
void SemanticValidatorVisitor::visitExpression(ExpressionNode* node) {}
void SemanticValidatorVisitor::visitFactor(FactorNode* node) {}
void SemanticValidatorVisitor::visitTerm(TermNode* node) {}
void SemanticValidatorVisitor::visitProcedure(ProcedureNode* node) {
    currentProcedure = node->name;
}
void SemanticValidatorVisitor::visitPrint(PrintNode* node) {}
void SemanticValidatorVisitor::visitVariable(VariableNode* node) {}
void SemanticValidatorVisitor::visitConstant(ConstantNode* node) {}
void SemanticValidatorVisitor::visitRead(ReadNode* node) {}
void SemanticValidatorVisitor::visitAssign(AssignmentNode* node) {}
void SemanticValidatorVisitor::visitWhile(WhileNode* node) {}
void SemanticValidatorVisitor::visitIf(IfNode* node) {}

void SemanticValidatorVisitor::visitCall(CallNode* node) {
    std::string calledProcedure = node->procedureName;
    // if procedure name of the call is not in the procedure name set, we throw an error
    if (procedureNamesPtr->find(calledProcedure) == procedureNamesPtr->end()) {
        throw NotCallingProcedureError(calledProcedure);
    }

    if (calledProcedure == currentProcedure) {
        throw NoRecursiveCallsAllowedError(calledProcedure);
    }
    (*procedureNamesPtr.get())[currentProcedure].push_back(calledProcedure);
}
