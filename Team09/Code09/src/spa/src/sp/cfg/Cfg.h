#pragma once
#include <unordered_map>
#include <vector>

#include "././pkb/PKBClient/PKBFacadeWriter.h"
#include "sp/ast/grammar_nodes/statements/StatementListNode.h"

class CFG {
public:
    CFG(PKBFacadeWriter* pkbWriter) {}
    explicit CFG() {}
    PKBFacadeWriter* pkbWriter;
    // Stores a mapping from line number to the possible next line numbers it can go to.
    std::unordered_map<int, std::vector<int>> parentToChildMap;
    void buildCFG(std::shared_ptr<ProcedureNode> procedure);
    void buildStatementListCFG(std::shared_ptr<StatementListNode> statementListNode, int nextLineNumber);
    void insertIntoCFGMap(int parentStatementNumber, int childStatementNumber);
};
