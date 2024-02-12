#include "PKB.h"
#include "PKB/Stores/EntityStore.h"
#include "PKB/Stores/StatementStore.h"

PKB::PKB() noexcept {
    variableStore = std::make_shared<EntityStore<Variable>>();
    constantStore = std::make_shared<EntityStore<Constant>>();
    procedureStore = std::make_shared<EntityStore<Procedure>>();

    stmtStore = std::make_shared<StatementStore>();

    followsStore = std::make_shared<FollowsStore>();
}
