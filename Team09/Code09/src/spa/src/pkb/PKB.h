#pragma once

#include <memory>

#include "pkb/Stores/AssignPatternStore.h"
#include "pkb/Stores/CallStore.h"
#include "pkb/Stores/EntityStore.h"
#include "pkb/Stores/FollowsStore.h"
#include "pkb/Stores/IfPatternStore.h"
#include "pkb/Stores/ModifiesStore.h"
#include "pkb/Stores/NextStore.h"
#include "pkb/Stores/ParentStore.h"
#include "pkb/Stores/StatementStore.h"
#include "pkb/Stores/UsesStore.h"
#include "pkb/Stores/WhilePatternStore.h"

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

    std::shared_ptr<NextStore> nextStore;
    std::shared_ptr<CallStore> callStore;
    std::shared_ptr<AssignPatternStore> assignPatternStore;
    std::shared_ptr<IfPatternStore> ifPatternStore;
    std::shared_ptr<WhilePatternStore> whilePatternStore;
};
