#include "ModifiesExtractor.h"

#include "NodeDeclarations.h"

void ModifiesExtractor::visitProgram(ProgramNode* node) {}
void ModifiesExtractor::visitStmtLst(StatementListNode* node) {}
void ModifiesExtractor::visitExpression(ExpressionNode* node) {}
void ModifiesExtractor::visitFactor(FactorNode* node) {}
void ModifiesExtractor::visitTerm(TermNode* node) {}
void ModifiesExtractor::visitPrint(PrintNode* node) {}
void ModifiesExtractor::visitVariable(VariableNode* node) {}
void ModifiesExtractor::visitConstant(ConstantNode* node) {}

void ModifiesExtractor::visitProcedure(ProcedureNode* node) {
    this->currentProc = node->name;
}

void ModifiesExtractor::visitCall(CallNode* node) {
    int modifierStmtNum = node->statementNumber;
    std::string calledProc = node->getCalledProcedure();
    std::unordered_set<std::pair<StmtNum, Variable>> extractedModifies;

    // if the called proc has been extracted before
    if (this->extractedProcs->find(calledProc) != this->extractedProcs->end()) {
        extractedModifies = extractedProcs->at(calledProc);
    } else {
        // Extract modifies relationships from modifiesExtractorHelper and append to ModifiesExtractor
        // Each modifies relationship will have the modifierStmtNum of the nested statement
        // So we simply replace the modifierStmtNum from the extracted modifies relationship
        // with the current CallNode stmtNum

        ProcedureNode* procNode = this->procs.at(calledProc);
        ModifiesExtractor* modifiesExtractorHelper =
            new ModifiesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        dfsVisitHelper(procNode, modifiesExtractorHelper);
        extractedModifies = modifiesExtractorHelper->getModifies();
        this->extractedProcs->insert({calledProc, extractedModifies});
        delete modifiesExtractorHelper;
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedModifies.begin(); it != extractedModifies.end();) {
        auto oldPair = *it;
        it = extractedModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->modifies.insert({modifierStmtNum, oldPair.second});
        this->procedureModifies.insert({currentProc, oldPair.second});
    }
}

void ModifiesExtractor::visitRead(ReadNode* node) {
    int modifierStmtNum = node->statementNumber;
    std::string modifiedVariable = node->variable->value;
    this->modifies.insert({modifierStmtNum, modifiedVariable});
    this->procedureModifies.insert({currentProc, modifiedVariable});
}

void ModifiesExtractor::visitAssign(AssignmentNode* node) {
    int modifierStmtNum = node->statementNumber;
    std::string modifiedVariable = node->variable->value;
    this->modifies.insert({modifierStmtNum, modifiedVariable});
    this->procedureModifies.insert({currentProc, modifiedVariable});
}

void ModifiesExtractor::visitWhile(WhileNode* node) {
    int modifierStmtNum = node->statementNumber;
    std::unordered_set<std::pair<StmtNum, Variable>> extractedModifies;

    // if this while statement has been extracted before
    if (extractedWhiles != nullptr && this->extractedWhiles->find(node) != this->extractedWhiles->end()) {
        extractedModifies = this->extractedWhiles->at(node);

    } else {
        ModifiesExtractor* modifiesExtractorHelper =
            new ModifiesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        modifiesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->whileStmtList, modifiesExtractorHelper);
        extractedModifies = modifiesExtractorHelper->getModifies();
        modifiesExtractorHelper->extractedWhiles->insert({node, extractedModifies});

        this->extractedWhiles->insert({node, extractedModifies});
        delete modifiesExtractorHelper;
    }

    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedModifies.begin(); it != extractedModifies.end();) {
        auto oldPair = *it;
        it = extractedModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->modifies.insert({modifierStmtNum, oldPair.second});
        this->procedureModifies.insert({currentProc, oldPair.second});
    }
}

void ModifiesExtractor::visitIf(IfNode* node) {
    int modifierStmtNum = node->statementNumber;
    std::unordered_set<std::pair<StmtNum, Variable>> extractedModifies;

    // if this if statement has been extracted before
    if (extractedIfs != nullptr && this->extractedIfs->find(node) != this->extractedIfs->end()) {
        extractedModifies = this->extractedIfs->at(node);
    } else {
        ModifiesExtractor* thenModifiesExtractorHelper =
            new ModifiesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        thenModifiesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->thenStmtList, thenModifiesExtractorHelper);
        for (const auto& elem : thenModifiesExtractorHelper->getModifies()) {
            extractedModifies.insert(elem);
        }

        ModifiesExtractor* elseModifiesExtractorHelper =
            new ModifiesExtractor(this->procs, this->extractedProcs, this->extractedWhiles, this->extractedIfs);
        elseModifiesExtractorHelper->currentProc = this->currentProc;
        dfsVisitHelper(node->elseStmtList, elseModifiesExtractorHelper);
        for (const auto& elem : elseModifiesExtractorHelper->getModifies()) {
            extractedModifies.insert(elem);
        }

        thenModifiesExtractorHelper->extractedIfs->insert({node, extractedModifies});
        elseModifiesExtractorHelper->extractedIfs->insert({node, extractedModifies});

        this->extractedIfs->insert({node, extractedModifies});
        delete thenModifiesExtractorHelper;
        delete elseModifiesExtractorHelper;
    }
    // Iterate over each element in the set and update stmtNum value
    for (auto it = extractedModifies.begin(); it != extractedModifies.end();) {
        auto oldPair = *it;
        it = extractedModifies.erase(it);

        // Create a new pair with the updated stmtNum value and insert it
        this->modifies.insert({modifierStmtNum, oldPair.second});
        this->procedureModifies.insert({currentProc, oldPair.second});
    }
}

std::unordered_set<std::pair<StmtNum, Variable>> ModifiesExtractor::getModifies() {
    return this->modifies;
}

std::unordered_set<std::pair<Procedure, Variable>> ModifiesExtractor::getProcedureModifies() {
    return this->procedureModifies;
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
