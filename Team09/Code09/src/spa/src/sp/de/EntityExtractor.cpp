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
    StatementType readStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::READ];
    this->statements.insert(Stmt{readStmt, stmtNum});
}

void EntityExtractor::visitPrint(PrintNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType printStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::PRINT];
    this->statements.insert(Stmt{printStmt, stmtNum});
}

void EntityExtractor::visitWhile(WhileNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType whileStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::WHILE];
    this->statements.insert(Stmt{whileStmt, stmtNum});
}

void EntityExtractor::visitIf(IfNode* node) {
    int stmtNum = node->getStmtNum();
    StatementType ifStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::IF];
    this->statements.insert(Stmt{ifStmt, stmtNum});
}

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

std::unordered_set<std::string> EntityExtractor::getVariables() {
    return this->variables;
}

std::unordered_set<std::string> EntityExtractor::getConstants() {
    return this->constants;
}

std::unordered_set<std::string> EntityExtractor::getProcedures() {
    return this->procedures;
}

std::unordered_set<Stmt> EntityExtractor::getStatements() {
    return this->statements;
}
