#pragma once

#include <memory>
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/VariablesStore.h"
#include "PKB/Stores/StatementStore.h"
#include "PKB/Stores/ConstantStore.h"
#include "PKB/Stores/ProcedureStore.h"

class PKB {
public:
    PKB() noexcept;
    std::shared_ptr<VariablesStore> variablesStore;
    std::shared_ptr<ConstantStore> constantStore;
    std::shared_ptr<ProcedureStore> procedureStore;

    std::shared_ptr<StatementStore> stmtStore;
    std::shared_ptr<FollowsStore> followsStore;
};
