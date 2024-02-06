#pragma once

#include <memory>
#include "PKB/Stores/FollowsStore.h"
#include "PKB/Stores/VariablesStore.h"

class PKB {
public:
    PKB() noexcept;
    std::shared_ptr<VariablesStore> variablesStore;
    std::shared_ptr<FollowsStore> followsStore;
};
