#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <new>
#include <type_traits>
#include <vector>

namespace qbs {

inline constexpr const char* version = "0.1.0";

// Quantum Bogo Sort (QBS) is an adaptive integer sorting experiment.
// Small ranges use std::sort; larger integral ranges use stable LSD radix sort.
// The "bogo" name is a joke: no unbounded random shuffling is performed.
template <std::contiguous_iterator It>
requires std::integral<std::iter_value_t<It>> &&
         (!std::same_as<std::remove_cv_t<std::iter_value_t<It>>, bool>)
void sort(It first, It last) {
    using T = std::iter_value_t<It>;
    using U = std::make_unsigned_t<T>;

    const auto distance = last - first;
    if (distance < 2) return;

    constexpr std::size_t small_range = 64;
    constexpr std::size_t radix = 256;
    constexpr unsigned bits_per_pass = 8;
    constexpr U sign_mask = [] {
        if constexpr (std::is_signed_v<T>)
            return U{1} << (sizeof(T) * 8 - 1);
        else
            return U{0};
    }();

    const auto n = static_cast<std::size_t>(distance);
    if (n < small_range) {
        std::sort(first, last);
        return;
    }

    // A bounded order probe: already-sorted input is a cheap fast path.
    if (std::is_sorted(first, last)) return;

    try {
        std::vector<T> buffer(n);
        T* source = std::to_address(first);
        T* destination = buffer.data();

        for (std::size_t pass = 0; pass < sizeof(T); ++pass) {
            std::array<std::size_t, radix> counts{};
            const unsigned shift = static_cast<unsigned>(pass) * bits_per_pass;

            for (std::size_t i = 0; i < n; ++i) {
                U key = static_cast<U>(source[i]) ^ sign_mask;
                ++counts[(key >> shift) & U{0xFF}];
            }

            std::array<std::size_t, radix> offsets{};
            std::size_t total = 0;
            for (std::size_t bucket = 0; bucket < radix; ++bucket) {
                offsets[bucket] = total;
                total += counts[bucket];
            }

            for (std::size_t i = 0; i < n; ++i) {
                U key = static_cast<U>(source[i]) ^ sign_mask;
                const auto digit = static_cast<std::size_t>((key >> shift) & U{0xFF});
                destination[offsets[digit]++] = source[i];
            }

            std::swap(source, destination);
        }

        // sizeof(T) passes are even for common 16/32/64-bit types, but
        // handle odd pass counts too rather than relying on that assumption.
        if (source != std::to_address(first))
            std::copy_n(source, n, first);
    } catch (const std::bad_alloc&) {
        // Preserve correctness under memory pressure; comparison-sort in place.
        std::sort(first, last);
    }
}

template <std::contiguous_iterator It>
requires std::integral<std::iter_value_t<It>> &&
         (!std::same_as<std::remove_cv_t<std::iter_value_t<It>>, bool>)
void quantum_bogo_sort(It first, It last) {
    sort(first, last);
}

} // namespace qbs
