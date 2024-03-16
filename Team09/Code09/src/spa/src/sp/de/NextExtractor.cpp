#include "NextExtractor.h"

#include "sp/de/AstVisitor.h"
std::unordered_set<std::pair<int, int>> NextExtractor::getNextRelationships() {
    std::unordered_set<std::pair<int, int>> nextRelationships;

    for (const auto& [key, values] : cfg.parentToChildMap) {
        for (auto value : values) {
            nextRelationships.insert(std::make_pair(key, value));
        }
    }
    return nextRelationships;
}

void NextExtractor::buildCFG(std::shared_ptr<ProcedureNode> procedure) {
    cfg.buildCFG(procedure);
}
