#include "PKB.h"

PKB::PKB() noexcept {
    variableStore = std::make_shared<EntityStore<Variable>>();
    constantStore = std::make_shared<EntityStore<Constant>>();
    procedureStore = std::make_shared<EntityStore<Procedure>>();

    stmtStore = std::make_shared<StatementStore>();

    followsStore = std::make_shared<FollowsStore>();
    parentStore = std::make_shared<ParentStore>();
    usesStore = std::make_shared<UsesStore>();
    modifiesStore = std::make_shared<ModifiesStore>();
    patternStore = std::make_shared<PatternStore>();
}
