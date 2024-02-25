#pragma once
/*
#include "../ast/grammar_nodes/ConstantNode.h"
#include "../ast/grammar_nodes/ExpressionNode.h"
#include "../ast/grammar_nodes/FactorNode.h"
#include "../ast/grammar_nodes/ProcedureNode.h"
#include "../ast/grammar_nodes/ProgramNode.h"
#include "../ast/grammar_nodes/TermNode.h"
#include "../ast/grammar_nodes/VariableNode.h"
#include "../ast/grammar_nodes/statements/AssignmentNode.h"
#include "../ast/grammar_nodes/statements/IfNode.h"
#include "../ast/grammar_nodes/statements/PrintNode.h"
#include "../ast/grammar_nodes/statements/ReadNode.h"
#include "../ast/grammar_nodes/statements/StatementListNode.h"
#include "../ast/grammar_nodes/statements/StatementNode.h"
#include "../ast/grammar_nodes/statements/WhileNode.h"
*/

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
