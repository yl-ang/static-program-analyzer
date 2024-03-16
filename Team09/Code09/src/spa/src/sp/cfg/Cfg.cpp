#include "Cfg.h"

#include <memory>

#include "sp/ast/grammar_nodes/ProcedureNode.h"
#include "sp/ast/grammar_nodes/statements/IfNode.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"
#include "sp/ast/grammar_nodes/statements/StatementNode.h"
#include "sp/ast/grammar_nodes/statements/WhileNode.h"
// builds the CFG for a single procedure.
void CFG::buildCFG(std::shared_ptr<ProcedureNode> procedure) {
    std::shared_ptr<StatementListNode> children = procedure->statements;
    buildStatementListCFG(children, 0);
}

void CFG::buildStatementListCFG(std::shared_ptr<StatementListNode> statementListNode, int loopLineStart) {
    std::vector<std::shared_ptr<StatementNode>> statementLists = statementListNode->children;
    // we need to look at the next statement number in case the current we are at is a while loop
    for (int i = 0; i < statementLists.size(); i++) {
        std::shared_ptr<StatementNode> currentNode = statementLists[i];
        int currentStatementNumber = currentNode->statementNumber;
        int nextStatementNumber = i + 1 == statementLists.size() ? 0 : statementLists[i + 1]->statementNumber;

        if (currentNode->getType() == "if") {
            std::shared_ptr<IfNode> ifNode = std::dynamic_pointer_cast<IfNode>(currentNode);
            std::shared_ptr<StatementListNode> thenNode = ifNode->thenStmtList;
            std::shared_ptr<StatementListNode> elseNode = ifNode->elseStmtList;
            insertIntoCFGMap(currentStatementNumber, thenNode->firstChildStatementNumber());
            insertIntoCFGMap(currentStatementNumber, elseNode->firstChildStatementNumber());
            // for the conditionals, if the next statement number is 0, it means that we have reached the end of the
            // block and that there are no statements left to process after this conditional statement. this must mean
            // we are in a loop block
            buildStatementListCFG(thenNode, nextStatementNumber == 0 ? loopLineStart : nextStatementNumber);
            buildStatementListCFG(elseNode, nextStatementNumber == 0 ? loopLineStart : nextStatementNumber);
        } else if (currentNode->getType() == "while") {
            std::shared_ptr<WhileNode> whileNode = std::dynamic_pointer_cast<WhileNode>(currentNode);
            // put the next first statement number of the while block into cfg map
            insertIntoCFGMap(currentStatementNumber, whileNode->whileStmtList->firstChildStatementNumber());
            // if there is a statement that is after the while loop, we need to add it to the cfg as well
            //
            if (nextStatementNumber) {
                insertIntoCFGMap(currentStatementNumber, nextStatementNumber);
            } else if (loopLineStart) {
                insertIntoCFGMap(currentStatementNumber, loopLineStart);
            }
            // for while, pass the currentStatementNumber as loopLineStart because that is the start of the loop.
            buildStatementListCFG(whileNode->whileStmtList, currentStatementNumber);
        } else {
            // if the next statement number is not 0, means that we still have some statements to go
            if (nextStatementNumber != 0) {
                insertIntoCFGMap(currentStatementNumber, nextStatementNumber);
            } else if (loopLineStart != 0) {
                insertIntoCFGMap(currentStatementNumber, loopLineStart);
            }
        }
    }
}

// attempts to insert the parent statement number into the CFG map.
// if the parent number does not exist, we create a new entry
// otherwise we add the child to the vector.
void CFG::insertIntoCFGMap(int parentStatementNumber, int childStatementNumber) {
    if (parentToChildMap.find(parentStatementNumber) == parentToChildMap.end()) {
        parentToChildMap[parentStatementNumber] = {childStatementNumber};
    } else {
        parentToChildMap[parentStatementNumber].push_back(childStatementNumber);
    }
}
