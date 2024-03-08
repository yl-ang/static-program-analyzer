#include <unordered_map>
#include <vector>

#include "././PKB/PKBClient/PKBFacadeWriter.h"
#include "sp/ast/grammar_nodes/ProcedureNode.h"

class CFG {
    PKBFacadeWriter* pkbWriter;
    // Stores a mapping from line number to the possible next line numbers it can go to.
    std::unordered_map<int, std::vector<int>> neighborMap;
    void buildCFG(std::shared_ptr<ProcedureNode>);
    CFG(PKBFacadeWriter* pkbWriter);
};
