#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "sp/de/AstVisitor.h"

class SemanticValidatorVisitor : public AstVisitor {
public:
    std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> procedureNamesPtr;
    std::string currentProcedure;
    SemanticValidatorVisitor(
        std::shared_ptr<std::unordered_map<std::string, std::vector<std::string>>> procedureNames) {
        this->procedureNamesPtr = procedureNames;
    }

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
