#include "sp/cfg/Cfg.h"
class NextExtractor {
    CFG cfg;

public:
    std::unordered_set<std::pair<int, int>> getNextRelationships();
    void buildCFG(std::shared_ptr<ASTNode> procedure);
};
