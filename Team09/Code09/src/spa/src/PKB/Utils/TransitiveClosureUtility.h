#pragma once

#include <stack>
#include <unordered_map>
#include <unordered_set>

template <typename KeyType>
class TransitiveClosureUtility {
public:
    static void computeTransitiveClosure(std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap);

private:
    static void computeClosureForKey(KeyType startNode, std::unordered_set<KeyType>* reachableNodes,
                                     std::unordered_map<KeyType, std::unordered_set<KeyType>>* starMap);
};

#include "TransitiveClosureUtility.tpp"