#pragma once

#include <memory>

#include "PKB/Stores/EntityStore.h"
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/ModifiesStore.h"
#include "PKB/Stores/ParentStore.h"
#include "PKB/Stores/PatternStore.h"
#include "PKB/Stores/StatementStore.h"
#include "PKB/Stores/UsesStore.h"

class PKB {
public:
    PKB() noexcept;

    std::shared_ptr<EntityStore<Variable>> variableStore;
    std::shared_ptr<EntityStore<Constant>> constantStore;
    std::shared_ptr<EntityStore<Procedure>> procedureStore;

    std::shared_ptr<StatementStore> stmtStore;

    std::shared_ptr<FollowsStore> followsStore;
    std::shared_ptr<ParentStore> parentStore;
    std::shared_ptr<UsesStore> usesStore;
    std::shared_ptr<ModifiesStore> modifiesStore;

    std::shared_ptr<PatternStore> patternStore;
};
