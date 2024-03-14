#include "sp/cfg/Cfg.h"
#include "sp/de/AstVisitor.h"
class NextExtractor {
    CFG cfg;

public:
    std::unordered_set<std::pair<int, int>> getNextRelationships();
    void buildCFG(std::shared_ptr<ProcedureNode> procedure);
};
