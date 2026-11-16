#include "qbs.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

template <class Sort>
long long measure(const std::vector<std::uint32_t>& input, Sort sort) {
    auto values = input;
    const auto start = std::chrono::steady_clock::now();
    sort(values.begin(), values.end());
    const auto stop = std::chrono::steady_clock::now();
    if (!std::is_sorted(values.begin(), values.end())) std::abort();
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}

int main(int argc, char** argv) {
    const std::size_t n = argc > 1 ? std::stoull(argv[1]) : 1'000'000;
    std::mt19937 rng(0x514253u);
    std::uniform_int_distribution<std::uint32_t> dist;
    std::vector<std::uint32_t> input(n);
    for (auto& value : input) value = dist(rng);

    const auto qbs_us = measure(input, [](auto first, auto last) {
        qbs::sort(first, last);
    });
    const auto std_us = measure(input, [](auto first, auto last) {
        std::sort(first, last);
    });

    std::cout << "n=" << n << '\n'
              << "QBS:     " << qbs_us << " us\n"
              << "std::sort: " << std_us << " us\n";
}
