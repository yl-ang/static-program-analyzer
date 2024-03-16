#pragma once

#include <unordered_map>
#include <unordered_set>

template <typename KeyType>
class TransitiveClosureUtility {
public:
    static void computeTransitiveClosure(std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap);
    static void updateTransitiveClosure(KeyType s1, KeyType s2,
                                        std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap);
};

#include "TransitiveClosureUtility.tpp"
