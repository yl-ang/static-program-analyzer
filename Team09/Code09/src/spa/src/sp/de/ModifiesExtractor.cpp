#include "ModifiesExtractor.h"

void ModifiesExtractor::visitProgram(ProgramNode* node) {}
void ModifiesExtractor::visitStmtLst(StatementListNode* node) {}
void ModifiesExtractor::visitExpression(ExpressionNode* node) {}
void ModifiesExtractor::visitFactor(FactorNode* node) {}
void ModifiesExtractor::visitTerm(TermNode* node) {}
void ModifiesExtractor::visitProcedure(ProcedureNode* node) {}
void ModifiesExtractor::visitPrint(PrintNode* node) {}
void ModifiesExtractor::visitVariable(VariableNode* node) {}
void ModifiesExtractor::visitConstant(ConstantNode* node) {}

void ModifiesExtractor::visitRead(ReadNode* node) {
    int modifierStmtNum = node->getStmtNum();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitAssign(AssignmentNode* node) {
    int modifierStmtNum = node->getStmtNum();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitWhile(WhileNode* node) {
    int modifierStmtNum = node->getStmtNum();
    ModifiesExtractor* modifiesExtractorHelper = new ModifiesExtractor();
    dfsVisitHelper(node->getStmtLstNode(), modifiesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedModifies = modifiesExtractorHelper.getModifies();

    // Iterate over each element in the set and update stmtNum value
    for (auto& pair : extractedModifies) {
        pair.first = modifierStmtNum;
        this->modifies.insert(pair);
    }
}

void ModifiesExtractor::visitIf(IfNode* node) {
    int modifierStmtNum = node->getStmtNum();
    ModifiesExtractor* thenModifiesExtractorHelper = new ModifiesExtractor();
    ModifiesExtractor* elseModifiesExtractorHelper = new ModifiesExtractor();
    dfsVisitHelper(node->getThenStmtLstNode(), thenModifiesExtractorHelper);
    dfsVisitHelper(node->getElseStmtLstNode(), elseModifiesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedThenModifies = thenModifiesExtractorHelper.getModifies();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedElseModifies = elseModifiesExtractorHelper.getModifies();

    // Iterate over each element in the set and update stmtNum value
    for (auto& pair : extractedThenModifies) {
        pair.first = modifierStmtNum;
        this->modifies.insert(pair);
    }
    for (auto& pair : extractedElseModifies) {
        pair.first = modifierStmtNum;
        this->modifies.insert(pair);
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> ModifiesExtractor::getModifies() {
    return this->modifies;
}

void ModifiesExtractor::dfsVisitHelper(std::unique_ptr<ASTNode> node, ModifiesExtractor* visitor) {
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisit(child, visitor);
    }
    return;
}
