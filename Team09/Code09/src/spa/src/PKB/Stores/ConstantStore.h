#pragma once
#include "EntityStore.h"
#include "PKB/Utils/DataTypes.h"

class ConstantStore : public EntityStore<Constant> {
public:
    bool containsConstant(const Variable &constant) const {
        return hasEntity(constant);
    }
};
