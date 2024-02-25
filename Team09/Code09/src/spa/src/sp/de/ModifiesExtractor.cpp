#include "ModifiesExtractor.h"

void ModifiesExtractor::visitRead(ReadNode* node) {
    int modifierStmtNum = node->getStmtNum();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitAssign(AssignmentNode* node) {
    int modifierStmtNum = node->getStmtNum();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitWhile(WhileNode* node) {}
void ModifiesExtractor::visitIf(IfNode* node) {}

void ModifiesExtractor::visitProgram(ProgramNode* node) {}
void ModifiesExtractor::visitStmtLst(StatementListNode* node) {}
void ModifiesExtractor::visitExpression(ExpressionNode* node) {}
void ModifiesExtractor::visitFactor(FactorNode* node) {}
void ModifiesExtractor::visitTerm(TermNode* node) {}
void ModifiesExtractor::visitProcedure(ProcedureNode* node) {}
void ModifiesExtractor::visitPrint(PrintNode* node) {}
void ModifiesExtractor::visitVariable(VariableNode* node) {}
void ModifiesExtractor::visitConstant(ConstantNode* node) {}

std::unordered_set<std::pair<StmtNum, Variable>> ModifiesExtractor::getModifies() {
    return this->modifies;
}
