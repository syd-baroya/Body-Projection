#pragma once
#include <vector>
template<typename T>
std::vector<T> sub_vector(std::vector<T> const& v, int m, int n) {
    auto first = v.begin() + m;
    auto last = v.begin() + n + 1;
    std::vector<T> vector(first, last);
    return vector;
}