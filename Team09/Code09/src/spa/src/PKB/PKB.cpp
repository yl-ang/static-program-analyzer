#include "PKB.h"

PKB::PKB() noexcept {
    followsStore = std::make_shared<FollowsStore>();
}
