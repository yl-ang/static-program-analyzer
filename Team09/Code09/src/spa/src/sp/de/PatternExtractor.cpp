#include "PatternExtractor.h"

#include <memory>

#include "NodeDeclarations.h"
#include "sp/PatternTreeNode.h"
#include "sp/Utils.h"

void PatternExtractor::visitPrint(PrintNode* node) {}
void PatternExtractor::visitWhile(WhileNode* node) {}
void PatternExtractor::visitIf(IfNode* node) {
    this->isInIfCondition = true;
}
void PatternExtractor::visitProgram(ProgramNode* node) {}
void PatternExtractor::visitStmtLst(StatementListNode* node) {
    this->isInIfCondition = false;
}
void PatternExtractor::visitExpression(ExpressionNode* node) {}
void PatternExtractor::visitFactor(FactorNode* node) {}
void PatternExtractor::visitTerm(TermNode* node) {}
void PatternExtractor::visitProcedure(ProcedureNode* node) {}
void PatternExtractor::visitRead(ReadNode* node) {}
void PatternExtractor::visitVariable(VariableNode* node) {
    if (this->isInIfCondition) {
        this->variablesInIfConditionExpr.insert({node->getStmtNumber(), node->value});
    }
}
void PatternExtractor::visitConstant(ConstantNode* node) {}
void PatternExtractor::visitCall(CallNode* node) {}

void PatternExtractor::visitAssign(AssignmentNode* node) {
    int stmtNum = node->statementNumber;
    std::string lhs = node->variable->value;
    std::shared_ptr<ExpressionNode> expr = node->expression;
    std::vector<std::string> rhsVars = expr->getVars();
    std::vector<std::string> rhsConsts = expr->getConsts();

    for (int i = 0; i < rhsVars.size(); ++i) {
        this->pattern.insert({stmtNum, {lhs, rhsVars[i]}});
    }
    for (int i = 0; i < rhsConsts.size(); ++i) {
        this->pattern.insert({stmtNum, {lhs, rhsConsts[i]}});
    }

    this->matchablePattern.insert({stmtNum, {lhs, expr}});

    // add to assignment traversal set
    // <LHS, RHS>, StmtNumber>
    std::string treeLHS = PatternTreeNode::serialiseToString(PatternTreeNode::buildTreeFromString(lhs));
    std::string treeRHS = PatternTreeNode::serialiseToString(PatternTreeNode::buildTreeFromAST(expr));
    this->assignmentTraversals.insert({stmtNum, {treeLHS, treeRHS}});
}

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> PatternExtractor::getPattern() {
    return this->pattern;
}

std::unordered_map<StmtNum, std::pair<std::string, std::shared_ptr<Matchable>>>
PatternExtractor::getMatchablePattern() {
    return this->matchablePattern;
}

std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> PatternExtractor::getAssignmentPattern() {
    return this->assignmentTraversals;
}

std::unordered_set<std::pair<StmtNum, std::string>> PatternExtractor::getIfPattern() {
    return this->variablesInIfConditionExpr;
}
