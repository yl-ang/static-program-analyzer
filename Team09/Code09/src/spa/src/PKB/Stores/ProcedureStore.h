#pragma once
#include "EntityStore.h"
#include "PKB/Utils/DataTypes.h"

class ProcedureStore : public EntityStore<Procedure> {
public:
    bool containsProcedure(const Procedure &procedure) const {
        return hasEntity(procedure);
    }
};
