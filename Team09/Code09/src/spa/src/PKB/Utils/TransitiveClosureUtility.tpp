template <typename KeyType>
void TransitiveClosureUtility<KeyType>::computeTransitiveClosure(
        std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap) {
    int size = starMap->size();

    for (int k = 1; k <= size; ++k) {
        for (const auto& entry : *starMap) {
            KeyType s1 = entry.first;
            for (const auto& s2 : entry.second) {
                updateTransitiveClosure(s1, s2, starMap);
            }
        }
    }
}

template<typename KeyType>
void TransitiveClosureUtility<KeyType>::updateTransitiveClosure(
        KeyType s1, KeyType s2, std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap) {
    if (starMap->count(s2)) {
        for (const auto& s3 : (*starMap)[s2]) {
            (*starMap)[s1].insert(s3);
        }
    }
}
