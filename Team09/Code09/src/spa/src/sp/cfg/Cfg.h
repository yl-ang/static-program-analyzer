#pragma once
#include <unordered_map>
#include <vector>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "sp/ast/AstNode.h"

class CFG {
public:
    CFG(PKBFacadeWriter* pkbWriter) {}
    explicit CFG() {}
    PKBFacadeWriter* pkbWriter;
    // Stores a mapping from line number to the possible next line numbers it can go to.
    std::unordered_map<int, std::vector<int>> parentToChildMap;
    std::unordered_set<std::pair<int, int>> nextRelationships;
    void buildCFG(std::shared_ptr<ASTNode> procedure);
    void buildStatementListCFG(std::shared_ptr<ASTNode> statementListNode, int nextLineNumber);
    void insertIntoCFGMap(int parentStatementNumber, int childStatementNumber);
    void createNextRelationships();
};
