#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "AbstractionExtractor.h"
#include "NodeDeclarations.h"

class ModifiesExtractor : public AbstractionExtractor {
public:
    ModifiesExtractor() {}
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

    std::unordered_set<std::pair<StmtNum, Variable>> getModifies();
    void ModifiesExtractor::dfsVisitHelper(std::unique_ptr<ASTNode> node, ModifiesExtractor* visitor);
};
