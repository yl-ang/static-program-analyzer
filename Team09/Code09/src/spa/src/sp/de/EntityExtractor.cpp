#include "EntityExtractor.h"

#include "NodeDeclarations.h"

void EntityExtractor::visitProgram(ProgramNode* node) {}
void EntityExtractor::visitStmtLst(StatementListNode* node) {}
void EntityExtractor::visitExpression(ExpressionNode* node) {}
void EntityExtractor::visitFactor(FactorNode* node) {}
void EntityExtractor::visitTerm(TermNode* node) {}
void EntityExtractor::visitCall(CallNode* node) {}

void EntityExtractor::visitProcedure(ProcedureNode* node) {
    std::string procedure = node->name;
    this->procedures.insert(procedure);
}

void EntityExtractor::visitRead(ReadNode* node) {
    int stmtNum = node->statementNumber;
    StatementType readStmt = DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::READ];
    this->statements.insert(Stmt{readStmt, stmtNum});
}

void EntityExtractor::visitPrint(PrintNode* node) {
    int stmtNum = node->statementNumber;
    StatementType printStmt = DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::PRINT];
    this->statements.insert(Stmt{printStmt, stmtNum});
}

void EntityExtractor::visitWhile(WhileNode* node) {
    int stmtNum = node->statementNumber;
    StatementType whileStmt = DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::WHILE];
    this->statements.insert(Stmt{whileStmt, stmtNum});
}

void EntityExtractor::visitIf(IfNode* node) {
    int stmtNum = node->statementNumber;
    StatementType ifStmt = DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::IF];
    this->statements.insert(Stmt{ifStmt, stmtNum});
}

void EntityExtractor::visitAssign(AssignmentNode* node) {
    int stmtNum = node->statementNumber;
    StatementType assign = DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP[DesignEntityStatementType::ASSIGN];
    this->statements.insert(Stmt{assign, stmtNum});
}

void EntityExtractor::visitVariable(VariableNode* node) {
    std::string var = node->value;
    this->variables.insert(var);
}

void EntityExtractor::visitConstant(ConstantNode* node) {
    std::string constant = node->value;
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
