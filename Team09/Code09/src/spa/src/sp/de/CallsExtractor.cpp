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
void CallsExtractor::visitCall(CallNode* node) {}
void CallsExtractor::visitWhile(WhileNode* node) {}
void CallsExtractor::visitIf(IfNode* node) {}

void CallsExtractor::visitProcedure(ProcedureNode* node) {
    std::string procedure = node->name;
    std::vector<std::shared_ptr<StatementNode>> callStmts = node->getStmtLstNode()->getCallStatements();
    if (callStmts.empty()) {
        // no call statements
        return;
    }
    for (auto callStmt : callStmts) {
        this->calls.insert({procedure, callStmt->getValue()});
    }
}

std::unordered_set<std::pair<Procedure, Procedure>> CallsExtractor::getCalls() {
    return this->calls;
}
