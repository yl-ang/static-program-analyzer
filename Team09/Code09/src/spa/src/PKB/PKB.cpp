#include "PKB.h"
#include "PKB/Stores/VariablesStore.h"
#include "PKB/Stores/StatementStore.h"

PKB::PKB() noexcept {
    variablesStore = std::make_shared<VariablesStore>();
    constantStore = std::make_shared<ConstantStore>();
    procedureStore = std::make_shared<ProcedureStore>();

    stmtStore = std::make_shared<StatementStore>();
    followsStore = std::make_shared<FollowsStore>();
}
