#include "ModifiesExtractor.h"

#include "NodeDeclarations.h"

void ModifiesExtractor::visitProgram(ProgramNode* node) {}
void ModifiesExtractor::visitStmtLst(StatementListNode* node) {}
void ModifiesExtractor::visitExpression(ExpressionNode* node) {}
void ModifiesExtractor::visitFactor(FactorNode* node) {}
void ModifiesExtractor::visitTerm(TermNode* node) {}
void ModifiesExtractor::visitProcedure(ProcedureNode* node) {}
void ModifiesExtractor::visitPrint(PrintNode* node) {}
void ModifiesExtractor::visitVariable(VariableNode* node) {}
void ModifiesExtractor::visitConstant(ConstantNode* node) {}
void ModifiesExtractor::visitCall(CallNode* node) {}

void ModifiesExtractor::visitRead(ReadNode* node) {
    int modifierStmtNum = node->getStmtNumber();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitAssign(AssignmentNode* node) {
    int modifierStmtNum = node->getStmtNumber();
    std::string modifiedVariable = node->getVar();
    this->modifies.insert({modifierStmtNum, modifiedVariable});
}

void ModifiesExtractor::visitWhile(WhileNode* node) {
    int modifierStmtNum = node->getStmtNumber();
    ModifiesExtractor* modifiesExtractorHelper = new ModifiesExtractor();
    dfsVisitHelper(node->getStmtLstNode(), modifiesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedModifies = modifiesExtractorHelper->getModifies();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedModifies.begin(); it != extractedModifies.end();) {
        auto oldPair = *it;
        it = extractedModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        modifies.insert({modifierStmtNum, oldPair.second});
    }
}

void ModifiesExtractor::visitIf(IfNode* node) {
    int modifierStmtNum = node->getStmtNumber();
    ModifiesExtractor* thenModifiesExtractorHelper = new ModifiesExtractor();
    ModifiesExtractor* elseModifiesExtractorHelper = new ModifiesExtractor();
    dfsVisitHelper(node->getThenStmtLstNode(), thenModifiesExtractorHelper);
    dfsVisitHelper(node->getElseStmtLstNode(), elseModifiesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedThenModifies = thenModifiesExtractorHelper->getModifies();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedElseModifies = elseModifiesExtractorHelper->getModifies();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedThenModifies.begin(); it != extractedThenModifies.end();) {
        auto oldPair = *it;
        it = extractedThenModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        modifies.insert({modifierStmtNum, oldPair.second});
    }

    for (auto it = extractedElseModifies.begin(); it != extractedElseModifies.end();) {
        auto oldPair = *it;
        it = extractedElseModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        modifies.insert({modifierStmtNum, oldPair.second});
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> ModifiesExtractor::getModifies() {
    return this->modifies;
}

void ModifiesExtractor::dfsVisitHelper(ASTNode* node, ModifiesExtractor* visitor) {
    node->accept(visitor);

    for (auto const& child : node->getChildren()) {
        dfsVisitHelper(child.get(), visitor);
    }
}

void ModifiesExtractor::dfsVisitHelper(std::shared_ptr<ASTNode> node, ModifiesExtractor* visitor) {
    node->accept(visitor);

    for (auto const& child : node->getChildren()) {
        dfsVisitHelper(child.get(), visitor);
    }
}
