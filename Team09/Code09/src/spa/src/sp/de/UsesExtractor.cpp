#include "UsesExtractor.h"

void UsesExtractor::visitAssign(AssignmentNode* node) {
    int userStmtNum = node->getStmtNum();
    std::vector<std::string> usedVariables = node->getExpr()->getVars();
    for (int i = 0; i < usedVariables.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariables[i]});
    }
}

void UsesExtractor::visitPrint(PrintNode* node) {
    int userStmtNum = node->getStmtNum();
    std::string usedVariable = node->getVar();
    this->uses.insert({userStmtNum, usedVariable});
}

void UsesExtractor::visitWhile(WhileNode* node) {
    int userStmtNum = node->getStmtNum();
    std::vector<std::string> usedVariablesInCond = node->getCond()->getVars();
    std::vector<std::string> usedVariablesInStmtLst;
    // Check the parent set to find all the child statements for the while statement,
    // then search those child statement numbers in
}

void UsesExtractor::visitIf(IfNode* node) {
    int userStmtNum = node->getStmtNum();
}

void UsesExtractor::visitProgram(ProgramNode* node) {}
void UsesExtractor::visitStmtLst(StatementListNode* node) {}
void UsesExtractor::visitExpression(ExpressionNode* node) {}
void UsesExtractor::visitFactor(FactorNode* node) {}
void UsesExtractor::visitTerm(TermNode* node) {}
void UsesExtractor::visitProcedure(ProcedureNode* node) {}
void UsesExtractor::visitRead(ReadNode* node) {}
void UsesExtractor::visitVariable(VariableNode* node) {}
void UsesExtractor::visitConstant(ConstantNode* node) {}

std::unordered_set<std::pair<StmtNum, Variable>> UsesExtractor::getUses() {
    return this->uses;
}
// container statements -> traverse the tree and get all the var nodes
