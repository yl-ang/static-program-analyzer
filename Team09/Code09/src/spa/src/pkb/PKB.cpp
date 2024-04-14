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

    nextStore = std::make_shared<NextStore>();
    callStore = std::make_shared<CallStore>();
    assignPatternStore = std::make_shared<AssignPatternStore>();
    ifPatternStore = std::make_shared<IfPatternStore>();
    whilePatternStore = std::make_shared<WhilePatternStore>();
}
