#pragma once

#include <memory>
#include "PKB/Stores/EntityStore.h"
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/StatementStore.h"

class PKB {
public:
    PKB() noexcept;

    std::shared_ptr<EntityStore<Variable>> variableStore;
    std::shared_ptr<EntityStore<Constant>> constantStore;
    std::shared_ptr<EntityStore<Procedure>> procedureStore;

    std::shared_ptr<StatementStore> stmtStore;
    std::shared_ptr<FollowsStore> followsStore;
};
