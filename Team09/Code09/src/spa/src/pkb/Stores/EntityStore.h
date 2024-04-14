#pragma once
#include <unordered_set>

// ai-gen start(gpt, 0, e)
// prompt:
// https://platform.openai.com/playground/p/JMwYQcYxmb857W2JkifHSp5w?model=gpt-4&mode=chat
template <typename T>
class EntityStore {
public:
    void addEntities(const std::unordered_set<T>& inputEntities);
    bool hasEntity(const T& entity) const;
    const std::unordered_set<T>& getAllEntities() const;

private:
    std::unordered_set<T> entities;
};

template <typename T>
void EntityStore<T>::addEntities(const std::unordered_set<T>& inputEntities) {
    entities.insert(inputEntities.begin(), inputEntities.end());
}

template <typename T>
bool EntityStore<T>::hasEntity(const T& entity) const {
    return entities.count(entity) > 0;
}

template <typename T>
const std::unordered_set<T>& EntityStore<T>::getAllEntities() const {
    return entities;
}
// ai-gen end
