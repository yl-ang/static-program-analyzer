#include "PKB.h"
#include "PKB/Stores/VariablesStore.h"

PKB::PKB() noexcept {
    followsStore = std::make_shared<FollowsStore>();
    variablesStore = std::make_shared<VariablesStore>();
}
