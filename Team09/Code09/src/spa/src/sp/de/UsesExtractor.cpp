#include "UsesExtractor.h"

#include "NodeDeclarations.h"

void UsesExtractor::visitProgram(ProgramNode* node) {}
void UsesExtractor::visitStmtLst(StatementListNode* node) {}
void UsesExtractor::visitExpression(ExpressionNode* node) {}
void UsesExtractor::visitFactor(FactorNode* node) {}
void UsesExtractor::visitTerm(TermNode* node) {}
void UsesExtractor::visitProcedure(ProcedureNode* node) {}
void UsesExtractor::visitRead(ReadNode* node) {}
void UsesExtractor::visitVariable(VariableNode* node) {}
void UsesExtractor::visitConstant(ConstantNode* node) {}

void UsesExtractor::visitCall(CallNode* node) {
    int userStmtNum = node->statementNumber;
    std::string calledProc = node->getCalledProcedure();
    ProcedureNode* procNode = this->procedures.at(calledProc);

    // Extract uses relationships from usesExtractorHelper and append to usesExtractor
    // Each uses relationship will have the userStmtNum of the nested statement
    // So we simply replace the userStmtNum from the extracted uses relationship
    // with the current CallNode stmtNum

    UsesExtractor* usesExtractorHelper = new UsesExtractor();
    dfsVisitHelper(procNode, usesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses = usesExtractorHelper->getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
    }
}

void UsesExtractor::visitAssign(AssignmentNode* node) {
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariables = node->expression->getVars();
    for (int i = 0; i < usedVariables.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariables[i]});
    }
}

void UsesExtractor::visitPrint(PrintNode* node) {
    int userStmtNum = node->statementNumber;
    std::string usedVariable = node->variable->value;
    this->uses.insert({userStmtNum, usedVariable});
}

void UsesExtractor::visitWhile(WhileNode* node) {
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariablesInCond = node->whileCondition->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
    }

    // Extract uses relationships from usesExtractorHelper and append to usesExtractor
    // Each uses relationship will have the userStmtNum of the nested statement
    // So we simply replace the userStmtNum from the extracted uses relationship
    // with the current WhileNode stmtNum

    UsesExtractor* usesExtractorHelper = new UsesExtractor();
    dfsVisitHelper(node->whileStmtList, usesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses = usesExtractorHelper->getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
    }
}

void UsesExtractor::visitIf(IfNode* node) {
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariablesInCond = node->ifCondition->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
    }
    UsesExtractor* thenUsesExtractorHelper = new UsesExtractor();
    UsesExtractor* elseUsesExtractorHelper = new UsesExtractor();
    dfsVisitHelper(node->thenStmtList, thenUsesExtractorHelper);
    dfsVisitHelper(node->elseStmtList, elseUsesExtractorHelper);

    std::unordered_set<std::pair<StmtNum, Variable>> extractedThenUses = thenUsesExtractorHelper->getUses();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedElseUses = elseUsesExtractorHelper->getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedThenUses.begin(); it != extractedThenUses.end();) {
        auto oldPair = *it;
        it = extractedThenUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
    }

    for (auto it = extractedElseUses.begin(); it != extractedElseUses.end();) {
        auto oldPair = *it;
        it = extractedElseUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> UsesExtractor::getUses() {
    return this->uses;
}

void UsesExtractor::dfsVisitHelper(std::shared_ptr<ASTNode> node, UsesExtractor* visitor) {
    node->accept(visitor);

    for (auto const& child : node->getChildren()) {
        dfsVisitHelper(child.get(), visitor);
    }
}

void UsesExtractor::dfsVisitHelper(ASTNode* node, UsesExtractor* visitor) {
    node->accept(visitor);

    for (auto& child : node->getChildren()) {
        dfsVisitHelper(child.get(), visitor);
    }
}

// per procedure,
// have a separate visitor that visits all the procedures, then stores the pointer to each procedure

// then in visitcall, call uses extractor on the procedure node of the procedure being called recursively
