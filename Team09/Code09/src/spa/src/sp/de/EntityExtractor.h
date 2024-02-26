#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../../PKB/PKBClient/PKBFacadeWriter.h"
#include "../../PKB/utils/DataTypes.h"
#include "AstVisitor.h"

enum class DesignEntityStatementType {
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
};

static std::unordered_map<DesignEntityStatementType, StatementType> DESIGN_ENTITY_STMT_TYPE_TO_STMT_TYPE_MAP = {
    {DesignEntityStatementType::ASSIGN, StatementType::ASSIGN},
    {DesignEntityStatementType::WHILE, StatementType::WHILE},
    {DesignEntityStatementType::IF, StatementType::IF},
    {DesignEntityStatementType::CALL, StatementType::CALL},
    {DesignEntityStatementType::READ, StatementType::READ},
    {DesignEntityStatementType::PRINT, StatementType::PRINT},
};

class EntityExtractor : public AstVisitor {
private:
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> constants;
    std::unordered_set<std::string> procedures;
    std::unordered_set<Stmt> statements;

public:
    EntityExtractor() {}

    void visitProgram(ProgramNode* node) override;
    void visitProcedure(ProcedureNode* node) override;
    void visitStmtLst(StatementListNode* node) override;
    void visitRead(ReadNode* node) override;
    void visitPrint(PrintNode* node) override;
    void visitWhile(WhileNode* node) override;
    void visitIf(IfNode* node) override;
    void visitAssign(AssignmentNode* node) override;
    void visitExpression(ExpressionNode* node) override;
    void visitFactor(FactorNode* node) override;
    void visitTerm(TermNode* node) override;
    void visitVariable(VariableNode* node) override;
    void visitConstant(ConstantNode* node) override;

    std::unordered_set<std::string> getVariables();
    std::unordered_set<std::string> getConstants();
    std::unordered_set<std::string> getProcedures();
    std::unordered_set<Stmt> getStatements();
};
