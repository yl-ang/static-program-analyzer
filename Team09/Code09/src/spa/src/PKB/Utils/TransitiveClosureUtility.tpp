template <typename KeyType>
void TransitiveClosureUtility<KeyType>::computeTransitiveClosure(
        std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap) {
    int size = starMap->size();

    // Precompute the closure for each key pair
    for (auto& entry : *starMap) {
        KeyType s1 = entry.first;
        std::unordered_set<KeyType> closure;
        computeClosureForKey(s1, &closure, starMap);
        entry.second = closure;
    }
}

template<typename KeyType>
void TransitiveClosureUtility<KeyType>::computeClosureForKey(
        KeyType startNode, std::unordered_set<KeyType>* reachableNodes,
        std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap) {
    std::stack<KeyType> stack;
    std::unordered_set<KeyType> visited;

    stack.push(startNode);
    visited.insert(startNode);

    while (!stack.empty()) {
        KeyType current = stack.top();
        stack.pop();
        for (const auto& next : (*starMap)[current]) {
            if (visited.find(next) == visited.end()) {
                reachableNodes->insert(next);
                stack.push(next);
                visited.insert(next);
            }
        }
    }
}
