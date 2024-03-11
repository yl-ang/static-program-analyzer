#pragma once
#include <memory>
#include <unordered_set>

#include "sp/de/AstVisitor.h"

class SemanticValidatorVisitor : public AstVisitor {
public:
    SemanticValidatorVisitor(std::shared_ptr<std::unordered_set<std::string>> procedureNames)
        : procedureNamesPtr(procedureNames) {}
    std::shared_ptr<std::unordered_set<std::string>> procedureNamesPtr;

protected:
    void visitStmtLst(StatementListNode* node) override;
    void visitProgram(ProgramNode* node) override;
    void visitProcedure(ProcedureNode* node) override;
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
    void visitCall(CallNode* node) override;
};
