#include "EntityExtractor.h"

void EntityExtractor::visitProgram(std::unique_ptr<ProgramNode> node) {}
void EntityExtractor::visitStmtLst(std::unique_ptr<StatementListNode> node) {}
void EntityExtractor::visitExpression(std::unique_ptr<ExpressionNode> node) {}
void EntityExtractor::visitFactor(std::unique_ptr<FactorNode> node) {}
void EntityExtractor::visitTerm(std::unique_ptr<TermNode> node) {}

void EntityExtractor::visitProcedure(std::unique_ptr<ProcedureNode> node) {
    std::string procedure = node->getValue();
    this->procedures.insert(procedure);
}

void EntityExtractor::visitRead(std::unique_ptr<ReadNode> node) {
    int stmtNum = node->getStmtNum();
    StatementType readStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::READ];
    this->statements.insert(Stmt{readStmt, stmtNum});
}

void EntityExtractor::visitPrint(std::unique_ptr<PrintNode> node) {
    int stmtNum = node->getStmtNum();
    StatementType printStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::PRINT];
    this->statements.insert(Stmt{printStmt, stmtNum});
}

void EntityExtractor::visitWhile(std::unique_ptr<WhileNode> node) {
    int stmtNum = node->getStmtNum();
    StatementType whileStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::WHILE];
    this->statements.insert(Stmt{whileStmt, stmtNum});
}

void EntityExtractor::visitIf(std::unique_ptr<IfNode> node) {
    int stmtNum = node->getStmtNum();
    StatementType ifStmt = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::IF];
    this->statements.insert(Stmt{ifStmt, stmtNum});
}

void EntityExtractor::visitAssign(std::unique_ptr<AssignmentNode> node) {
    int stmtNum = node->getStmtNum();
    StatementType assign = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::ASSIGN];
    this->statements.insert(Stmt{assign, stmtNum});
}

void EntityExtractor::visitVariable(std::unique_ptr<VariableNode> node) {
    std::string var = node->getValue();
    this->variables.insert(var);
}

void EntityExtractor::visitConstant(std::unique_ptr<ConstantNode> node) {
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
