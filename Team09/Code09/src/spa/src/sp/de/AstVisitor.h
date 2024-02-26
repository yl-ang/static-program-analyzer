#pragma once

class ProgramNode;
class ProcedureNode;
class StatementListNode;
class ReadNode;
class PrintNode;
class WhileNode;
class IfNode;
class AssignmentNode;
class ExpressionNode;
class FactorNode;
class TermNode;
class VariableNode;
class ConstantNode;

// Visitor interface for Design Extractor
class AstVisitor {
public:
    virtual void visitProgram(ProgramNode* node) = 0;
    virtual void visitProcedure(ProcedureNode* node) = 0;
    virtual void visitStmtLst(StatementListNode* node) = 0;
    virtual void visitRead(ReadNode* node) = 0;
    virtual void visitPrint(PrintNode* node) = 0;
    virtual void visitWhile(WhileNode* node) = 0;
    virtual void visitIf(IfNode* node) = 0;
    virtual void visitAssign(AssignmentNode* node) = 0;
    virtual void visitExpression(ExpressionNode* node) = 0;
    virtual void visitFactor(FactorNode* node) = 0;
    virtual void visitTerm(TermNode* node) = 0;
    virtual void visitVariable(VariableNode* node) = 0;
    virtual void visitConstant(ConstantNode* node) = 0;
};
