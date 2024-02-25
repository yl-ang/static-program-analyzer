#pragma once
#include <memory>
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

static std::unordered_map<DesignEntityStatementType, StatementType> DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP = {
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

    void visitProgram(std::unique_ptr<ProgramNode> node) override;
    void visitProcedure(std::unique_ptr<ProcedureNode> node) override;
    void visitStmtLst(std::unique_ptr<StatementListNode> node) override;
    void visitRead(std::unique_ptr<ReadNode> node) override;
    void visitPrint(std::unique_ptr<PrintNode> node) override;
    void visitWhile(std::unique_ptr<WhileNode> node) override;
    void visitIf(std::unique_ptr<IfNode> node) override;
    void visitAssign(std::unique_ptr<AssignmentNode> node) override;
    void visitExpression(std::unique_ptr<ExpressionNode> node) override;
    void visitFactor(std::unique_ptr<FactorNode> node) override;
    void visitTerm(std::unique_ptr<TermNode> node) override;
    void visitVariable(std::unique_ptr<VariableNode> node) override;
    void visitConstant(std::unique_ptr<ConstantNode> node) override;

    std::unordered_set<std::string> getVariables();
    std::unordered_set<std::string> getConstants();
    std::unordered_set<std::string> getProcedures();
    std::unordered_set<Stmt> getStatements();
};
