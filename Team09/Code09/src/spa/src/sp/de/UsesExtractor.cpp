#include "UsesExtractor.h"

void UsesExtractor::visitProgram(std::unique_ptr<ProgramNode> node) {}
void UsesExtractor::visitStmtLst(std::unique_ptr<StatementListNode> node) {}
void UsesExtractor::visitExpression(std::unique_ptr<ExpressionNode> node) {}
void UsesExtractor::visitFactor(Fstd::unique_ptr<actorNode> node) {}
void UsesExtractor::visitTerm(std::unique_ptr<TermNode> node) {}
void UsesExtractor::visitProcedure(std::unique_ptr<ProcedureNode> node) {}
void UsesExtractor::visitRead(std::unique_ptr<ReadNode> node) {}
void UsesExtractor::visitVariable(std::unique_ptr<VariableNode> node) {}
void UsesExtractor::visitConstant(std::unique_ptr<ConstantNode> node) {}

void UsesExtractor::visitAssign(std::unique_ptr<AssignmentNode> node) {
    int userStmtNum = node->getStmtNum();
    std::vector<std::string> usedVariables = node->getExpr()->getVars();
    for (int i = 0; i < usedVariables.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariables[i]});
    }
}

void UsesExtractor::visitPrint(std::unique_ptr<PrintNode> node) {
    int userStmtNum = node->getStmtNum();
    std::string usedVariable = node->getVar();
    this->uses.insert({userStmtNum, usedVariable});
}

void UsesExtractor::visitWhile(std::unique_ptr<WhileNode> node) {
    int userStmtNum = node->getStmtNum();
    std::vector<std::string> usedVariablesInCond = node->getCond()->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
    }

    UsesExtractor* usesExtractorHelper = new UsesExtractor();
    dfsVisitHelper(node->getStmtLstNode(), usesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses = usesExtractorHelper.getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto& pair : extractedUses) {
        pair.first = userStmtNum;
        this->uses.insert(pair);
    }

    // Extract uses relationships from usesExtractorHelper and append to usesExtractor
    // Each uses relationship will have the userStmtNum of the nested statement
    // So we simply replace the userStmtNum from the extracted uses relationship
    // with the current WhileNode stmtNum
    // delete usesExtractorHelper
}

void UsesExtractor::visitIf(std::unique_ptr<IfNode> node) {
    int userStmtNum = node->getStmtNum();
    std::vector<std::string> usedVariablesInCond = node->getCond()->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
    }
    UsesExtractor* thenUsesExtractorHelper = new UsesExtractor();
    UsesExtractor* elseUsesExtractorHelper = new UsesExtractor();
    dfsVisitHelper(node->getThenStmtLst(), thenUsesExtractorHelper);
    dfsVisitHelper(node->getElseStmtLst(), elseUsesExtractorHelper);

    std::unordered_set<std::pair<StmtNum, Variable>> extractedThenUses = thenUsesExtractorHelper.getUses();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedElseUses = elseUsesExtractorHelper.getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto& pair : extractedThenUses) {
        pair.first = userStmtNum;
        this->uses.insert(pair);
    }
    for (auto& pair : extractedElseUses) {
        pair.first = userStmtNum;
        this->uses.insert(pair);
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> UsesExtractor::getUses() {
    return this->uses;
}

void UsesExtractor::dfsVisitHelper(std::unique_ptr<ASTNode> node, UsesExtractor* visitor) {
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisit(child, visitor);
    }
    return;
}
