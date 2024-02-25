#include "EntityExtractor.h"

void EntityExtractor::visitProgram(ProgramNode* node) {}
void EntityExtractor::visitStmtLst(StatementListNode* node) {}
void EntityExtractor::visitExpression(ExpressionNode* node) {}
void EntityExtractor::visitFactor(FactorNode* node) {}
void EntityExtractor::visitTerm(TermNode* node) {}

void EntityExtractor::visitProcedure(ProcedureNode* node) {
    std::string procedure = node->getValue();
    this->procedures.insert(procedure);
}

void EntityExtractor::visitRead(ReadNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType read = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::READ];
    this->statements.insert(Stmt{read, stmtNum});
}

void EntityExtractor::visitPrint(PrintNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType print = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::PRINT];
    this->statements.insert(Stmt{print, stmtNum});
}

// void EntityExtractor::visitWhile(WhileNode* node) {}
// void EntityExtractor::visitIf(IfNode* node) {}

void EntityExtractor::visitAssign(AssignmentNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType assign = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::ASSIGN];
    this->statements.insert(Stmt{assign, stmtNum});
}

void EntityExtractor::visitVariable(VariableNode* node) {
    std::string var = node->getValue();
    this->variables.insert(var);
}

void EntityExtractor::visitConstant(ConstantNode* node) {
    std::string constant = node->getValue();
    this->constants.insert(constant);
}
