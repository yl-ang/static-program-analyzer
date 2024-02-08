#pragma once
#include "EntityStore.h"
#include "PKB/Utils/DataTypes.h"

// ai-gen start(gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
class VariablesStore : public EntityStore<Variable> {
public:
    bool containsVariable(const Variable &variable) const {
        return hasEntity(variable);
    }
};
