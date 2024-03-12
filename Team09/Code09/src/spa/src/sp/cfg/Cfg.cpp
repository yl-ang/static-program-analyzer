#include "Cfg.h"

#include <memory>

#include "sp/ast/AstNode.h"
// builds the CFG for a single procedure.
void CFG::buildCFG(std::shared_ptr<ASTNode> procedure) {
    std::vector<std::shared_ptr<ASTNode>> children = procedure->getChildren();
    buildStatementListCFG(children[0], 0);
}

void CFG::buildStatementListCFG(std::shared_ptr<ASTNode> statementListNode, int loopLineStart) {
    std::vector<std::shared_ptr<ASTNode>> statementLists = statementListNode->getChildren();
    int lastStatementNumber = statementLists.back()->getStmtNumber();
    // we need to look at the next statement number in case the current we are at is a while loop
    for (int i = 0; i < statementLists.size(); i++) {
        std::shared_ptr<ASTNode> currentNode = statementLists[i];
        int currentStatementNumber = currentNode->getStmtNumber();
        int nextStatementNumber =
            lastStatementNumber == currentStatementNumber ? 0 : statementLists[i + 1]->getStmtNumber();
        if (currentNode->getType() == "if") {
            std::shared_ptr<ASTNode> thenNode = currentNode->getChildren()[1];
            std::shared_ptr<ASTNode> elseNode = currentNode->getChildren()[2];
            insertIntoCFGMap(currentStatementNumber, thenNode->getChildren()[0]->getStmtNumber());
            insertIntoCFGMap(currentStatementNumber, elseNode->getChildren()[0]->getStmtNumber());
            // for the conditionals, if the next statement number is 0, it means that we have reached the end of the
            // block and that there are no statements left to process after this conditional statement. this must mean
            // we are in a loop block
            buildStatementListCFG(thenNode, nextStatementNumber == 0 ? loopLineStart : nextStatementNumber);
            buildStatementListCFG(elseNode, nextStatementNumber == 0 ? loopLineStart : nextStatementNumber);
        } else if (currentNode->getType() == "while") {
            // put the next first statement number of the while block into cfg map
            insertIntoCFGMap(currentStatementNumber, currentNode->getChildren()[1]->getChildren()[0]->getStmtNumber());
            // if there is a statement that is after the while loop, we need to add it to the cfg as well
            //
            if (nextStatementNumber != 0) {
                insertIntoCFGMap(currentStatementNumber, nextStatementNumber);
            } else if (loopLineStart != 0) {
                insertIntoCFGMap(currentStatementNumber, loopLineStart);
            }
            // for while, pass the currentStatementNumber as loopLineStart because that is the start of the loop.
            buildStatementListCFG(currentNode->getChildren()[1], currentStatementNumber);
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
