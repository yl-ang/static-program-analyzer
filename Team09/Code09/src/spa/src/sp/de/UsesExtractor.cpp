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
    int userStmtNum = node->statementNumber;
    std::string calledProc = node->getCalledProcedure();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses;

    // if the called proc has been extracted before
    if (this->extractedProcs->find(calledProc) != this->extractedProcs->end()) {
        extractedUses = this->extractedProcs->at(calledProc);

    } else {
        // Extract uses relationships from usesExtractorHelper and append to usesExtractor
        // Each uses relationship will have the userStmtNum of the nested statement
        // So we simply replace the userStmtNum from the extracted uses relationship
        // with the current CallNode stmtNum
        ProcedureNode* procNode = this->procs.at(calledProc);
        UsesExtractor* usesExtractorHelper =
            new UsesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);

        dfsVisitHelper(procNode, usesExtractorHelper);
        extractedUses = usesExtractorHelper->getUses();
        usesExtractorHelper->extractedProcs->insert({calledProc, extractedUses});
        usesExtractorHelper->currentProc = this->currentProc;
        delete usesExtractorHelper;
        this->extractedProcs->insert({calledProc, extractedUses});
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->uses.insert({userStmtNum, oldPair.second});
        this->procedureUses.insert({currentProc, oldPair.second});
    }
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
#ifdef DEBUG_BUILD
    std::cout << "Visiting While" << std::endl;
#endif
    int userStmtNum = node->statementNumber;
    std::vector<std::string> usedVariablesInCond = node->whileCondition->getVars();
    for (int i = 0; i < usedVariablesInCond.size(); ++i) {
        this->uses.insert({userStmtNum, usedVariablesInCond[i]});
        this->procedureUses.insert({currentProc, usedVariablesInCond[i]});
    }
    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses;

#ifdef DEBUG_BUILD
    std::cout << "trying to access while cache" << std::endl;
#endif
    // if this while statement has been extracted before
    if (extractedWhiles != nullptr && this->extractedWhiles->find(node) != this->extractedWhiles->end()) {
        extractedUses = this->extractedWhiles->at(node);
#ifdef DEBUG_BUILD
        std::cout << "accessing while cache" << std::endl;
#endif
    } else {
#ifdef DEBUG_BUILD
        std::cout << "no while cached" << std::endl;
#endif

        UsesExtractor* usesExtractorHelper =
            new UsesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        usesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->whileStmtList, usesExtractorHelper);
        extractedUses = usesExtractorHelper->getUses();
        usesExtractorHelper->extractedWhiles->insert({node, extractedUses});

        this->extractedWhiles->insert({node, extractedUses});
        delete usesExtractorHelper;
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->uses.insert({userStmtNum, oldPair.second});
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

    std::unordered_set<std::pair<StmtNum, Variable>> extractedUses;

    // if this if statement has been extracted before
    if (extractedIfs != nullptr && this->extractedIfs->find(node) != this->extractedIfs->end()) {
        extractedUses = this->extractedIfs->at(node);
#ifdef DEBUG_BUILD
        std::cout << "accessing if cache" << std::endl;
#endif
    } else {
#ifdef DEBUG_BUILD
        std::cout << "no if cached" << std::endl;
#endif
        UsesExtractor* thenUsesExtractorHelper =
            new UsesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        thenUsesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->thenStmtList, thenUsesExtractorHelper);
        for (const auto& elem : thenUsesExtractorHelper->getUses()) {
            extractedUses.insert(elem);
        }

        UsesExtractor* elseUsesExtractorHelper =
            new UsesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        elseUsesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->elseStmtList, elseUsesExtractorHelper);
        for (const auto& elem : elseUsesExtractorHelper->getUses()) {
            extractedUses.insert(elem);
        }

        thenUsesExtractorHelper->extractedIfs->insert({node, extractedUses});
        elseUsesExtractorHelper->extractedIfs->insert({node, extractedUses});

        this->extractedIfs->insert({node, extractedUses});
        delete thenUsesExtractorHelper;
        delete elseUsesExtractorHelper;
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedUses.begin(); it != extractedUses.end();) {
        auto oldPair = *it;
        it = extractedUses.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->uses.insert({userStmtNum, oldPair.second});
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
