#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

#include <vector>
#include <unordered_map>
#include "StockOrder.h"

template<typename T, typename any>
std::vector<T> get_keys(std::unordered_map<T, any> hashtable) {
    std::vector<T> result;
    result.reserve(hashtable.size());
    for(auto kv : hashtable) {
      result.push_back(kv.first);
    }
    return result;
}

template<typename any, typename T>
std::vector<T> get_values(std::unordered_map<any, T> hashtable) {
    std::vector<T> result;
    result.reserve(hashtable.size());
    for(auto kv : hashtable) {
      result.push_back(kv.second);
    }
    return result;
}

#endif /* end of include guard: SRC_UTILITY_H_ */
