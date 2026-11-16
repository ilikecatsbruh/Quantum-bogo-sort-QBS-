#include "qbs.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

template <class T>
void check(std::vector<T> values) {
    auto expected = values;
    std::sort(expected.begin(), expected.end());
    qbs::sort(values.begin(), values.end());
    assert(values == expected);
}

int main() {
    check(std::vector<int>{});
    check(std::vector<int>{1});
    check(std::vector<int>{2, 1});
    check(std::vector<int>{5, -1, 0, -9, 5, 2, -1});
    check(std::vector<int>{42, 42, 42, 42});
    check(std::vector<int>{std::numeric_limits<int>::max(),
                           std::numeric_limits<int>::min(), 0, -1, 1});
    check(std::vector<std::uint64_t>{0, UINT64_MAX, 1, 1ULL << 63, 17});

    std::mt19937_64 rng(0x514253u);
    std::uniform_int_distribution<int> dist(
        std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (int trial = 0; trial < 100; ++trial) {
        std::vector<int> values(static_cast<std::size_t>(trial) * 137);
        for (auto& value : values) value = dist(rng);
        check(std::move(values));
    }

    std::vector<int> sorted(10000);
    for (std::size_t i = 0; i < sorted.size(); ++i)
        sorted[i] = static_cast<int>(i);
    qbs::sort(sorted.begin(), sorted.end());
    assert(std::is_sorted(sorted.begin(), sorted.end()));
}
