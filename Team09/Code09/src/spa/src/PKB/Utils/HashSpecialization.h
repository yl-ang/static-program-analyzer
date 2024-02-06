#include <functional>
#include <string>

#ifndef PAIR_HASH_H
#define PAIR_HASH_H

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 * 31 + hash2;
    }
};

#endif // PAIR_HASH_H
