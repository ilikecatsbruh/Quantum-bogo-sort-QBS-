# Quantum Bogo Sort (QBS)

**Quantum Bogo Sort** is an experimental C++20 sorting project with a deliberately ridiculous name and a practical goal: build a fast, measurable sorting implementation.

## Current implementation (v0.1.0)

QBS currently uses an adaptive strategy:

- Ranges shorter than 64 elements use `std::sort`.
- Already-sorted input returns immediately.
- Larger integral ranges use stable LSD radix sorting with 8-bit digits.
- Signed integers are mapped into unsigned key space by flipping the sign bit, preserving numeric order.
- If the temporary buffer cannot be allocated, QBS falls back to `std::sort`.

The algorithm is deterministic. "Bogo" is branding, not an instruction to randomly shuffle data until it happens to be sorted. No quantum hardware or quantum speedup is involved.

## Requirements

- C++20 compiler
- CMake 3.20+

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Run the benchmark (default: one million 32-bit integers):

```sh
./build/qbs_benchmark
./build/qbs_benchmark 5000000
```

## API

```cpp
#include "qbs.hpp"
#include <vector>

int main() {
    std::vector<int> values{8, -2, 4, 4, 0};
    qbs::sort(values.begin(), values.end());
}
```

The current API accepts contiguous iterators over non-boolean integral types.

## Performance status

QBS is a prototype, **not yet proven faster than `std::sort`**. Radix sorting can be attractive for large integer arrays, but its extra memory traffic and allocation costs can make it slower on some machines or distributions. Run the included benchmark on your target hardware before drawing conclusions.

## Roadmap

- Add distribution-aware benchmarks (sorted, reverse, duplicate-heavy, narrow-range, random).
- Compare against a high-quality radix sort and pattern-defeating quicksort.
- Tune small-range thresholds and digit widths using repeatable measurements.
- Explore bounded sampling and partition-based hybrid paths without sacrificing worst-case safety.
- Add support for custom key extraction and additional iterator categories.

## License

MIT
