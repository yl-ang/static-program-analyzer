#include "UsesExtractor.h"

#include "NodeDeclarations.h"

void UsesExtractor::visitProgram(ProgramNode* node) {}
void UsesExtractor::visitStmtLst(StatementListNode* node) {}
void UsesExtractor::visitExpression(ExpressionNode* node) {}
void UsesExtractor::visitFactor(FactorNode* node) {}
void UsesExtractor::visitTerm(TermNode* node) {}
void UsesExtractor::visitRead(ReadNode* node) {}
void UsesExtractor::visitVariable(VariableNode* node) {}
void UsesExtractor::visitConstant(ConstantNode* node) {}

void UsesExtractor::visitProcedure(ProcedureNode* node) {
#ifdef DEBUG_BUILD
    std::cout << "Visiting Procedure " << node->name << std::endl;
#endif
    this->currentProc = node->name;
}

void UsesExtractor::visitCall(CallNode* node) {
#ifdef DEBUG_BUILD
    std::cout << "Visiting Call" << std::endl;
#endif
    int userStmtNum = node->statementNumber;
    std::string calledProc = node->getCalledProcedure();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses;

#ifdef DEBUG_BUILD
    std::cout << currentProc << " is calling " << calledProc << std::endl;
#endif

    // if the called proc has been extracted before
    if (this->extractedProcs->find(calledProc) != this->extractedProcs->end()) {
        extractedUses = this->extractedProcs->at(calledProc);
#ifdef DEBUG_BUILD
        std::cout << "Accessing cached data for " << calledProc << std::endl;
#endif
    } else {
        // Extract uses relationships from usesExtractorHelper and append to usesExtractor
        // Each uses relationship will have the userStmtNum of the nested statement
        // So we simply replace the userStmtNum from the extracted uses relationship
        // with the current CallNode stmtNum
        ProcedureNode* procNode = this->procs.at(calledProc);
        UsesExtractor* usesExtractorHelper = new UsesExtractor(this->procs, this->extractedProcs);

        dfsVisitHelper(procNode, usesExtractorHelper);
        extractedUses = usesExtractorHelper->getUses();
        usesExtractorHelper->extractedProcs->insert({calledProc, extractedUses});
        usesExtractorHelper->currentProc = this->currentProc;

#ifdef DEBUG_BUILD
        std::cout << "Caching extracted uses for " << calledProc << std::endl;
#endif
        this->extractedProcs->insert({calledProc, extractedUses});

#ifdef DEBUG_BUILD
        std::cout << "Extracted Procs size: " << this->extractedProcs->size() << std::endl;
#endif
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->uses.insert({userStmtNum, oldPair.second});
        this->procedureUses.insert({currentProc, oldPair.second});
    }

#ifdef DEBUG_BUILD
    std::cout << "After iteration: Extracted Procs size: " << this->extractedProcs->size() << std::endl;
#endif
}

void UsesExtractor::visitAssign(AssignmentNode* node) {
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariables = node->expression->getVars();
    for (int i = 0; i < usedVariables.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariables[i]});
        this->procedureUses.insert({currentProc, usedVariables[i]});
    }
}

void UsesExtractor::visitPrint(PrintNode* node) {
    int userStmtNum = node->statementNumber;
    std::string usedVariable = node->variable->value;
    this->uses.insert({userStmtNum, usedVariable});
    this->procedureUses.insert({currentProc, usedVariable});
}

void UsesExtractor::visitWhile(WhileNode* node) {
    // #ifdef DEBUG_BUILD
    //     std::cout << "Visiting While" << std::endl;
    // #endif
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariablesInCond = node->whileCondition->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
        this->procedureUses.insert({currentProc, usedVariablesInCond[i]});
    }

    // Extract uses relationships from usesExtractorHelper and append to usesExtractor
    // Each uses relationship will have the userStmtNum of the nested statement
    // So we simply replace the userStmtNum from the extracted uses relationship
    // with the current WhileNode stmtNum

    UsesExtractor* usesExtractorHelper = new UsesExtractor(this->procs, this->extractedProcs);
    usesExtractorHelper->currentProc = this->currentProc;
    dfsVisitHelper(node->whileStmtList, usesExtractorHelper);
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses = usesExtractorHelper->getUses();

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
        this->procedureUses.insert({currentProc, oldPair.second});
    }
}

void UsesExtractor::visitIf(IfNode* node) {
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariablesInCond = node->ifCondition->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
        this->procedureUses.insert({currentProc, usedVariablesInCond[i]});
    }
    UsesExtractor* thenUsesExtractorHelper = new UsesExtractor(this->procs, this->extractedProcs);
    thenUsesExtractorHelper->currentProc = this->currentProc;
    UsesExtractor* elseUsesExtractorHelper = new UsesExtractor(this->procs, this->extractedProcs);
    elseUsesExtractorHelper->currentProc = this->currentProc;
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
        this->procedureUses.insert({currentProc, oldPair.second});
    }

    for (auto it = extractedElseUses.begin(); it != extractedElseUses.end();) {
        auto oldPair = *it;
        it = extractedElseUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        uses.insert({userStmtNum, oldPair.second});
        this->procedureUses.insert({currentProc, oldPair.second});
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> UsesExtractor::getUses() {
    return this->uses;
}

std::unordered_set<std::pair<Procedure, Variable>> UsesExtractor::getProcedureUses() {
    return this->procedureUses;
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
