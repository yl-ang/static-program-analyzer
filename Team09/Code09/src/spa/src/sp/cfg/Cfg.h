#include <unordered_map>
#include <vector>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"

class CFG {
    PKBFacadeWriter* pkbWriter;
    // Stores a mapping from line number to the possible next line numbers it can go to.
    std::unordered_map<int, std::vector<int>> parentToChildMap;
    void buildCFG(std::shared_ptr<ProcedureNode> procedure);
    CFG(PKBFacadeWriter* pkbWriter);
    void buildStatementListCFG(std::shared_ptr<ASTNode> statementListNode, int nextLineNumber);
    void insertIntoCFGMap(int parentStatementNumber, int childStatementNumber);
};
