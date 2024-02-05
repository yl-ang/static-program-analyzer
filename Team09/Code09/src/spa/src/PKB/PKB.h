#pragma once

#include <memory>
#include "PKB/Stores/FollowsStore.h"

class PKB {
public:
    PKB() noexcept;  // Add noexcept to the constructor declaration

private:
    std::shared_ptr<FollowsStore> followsStore;
};
