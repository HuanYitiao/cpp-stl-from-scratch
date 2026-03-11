# stl-from-scratch

A from-scratch implementation of C++ STL containers, built for deep understanding of data structures and memory management.

## Containers

| # | Container | Status |
|---|-----------|--------|
| 1 | `Vector` | 🔨 In Progress |
| 2 | `List` | ⬜ |
| 3 | `Deque` | ⬜ |
| 4 | `HashTable` | ⬜ |
| 5 | `Red-Black Tree` | ⬜ |
| 6 | `Set` | ⬜ |
| 7 | `unordered_set` | ⬜ |
| 8 | `multiset` | ⬜ |
| 9 | `Map` | ⬜ |
| 10 | `unordered_map` | ⬜ |
| 11 | `multimap` | ⬜ |
| 12 | `Stack` | ⬜ |
| 13 | `Queue` | ⬜ |
| 14 | `priority_queue` | ⬜ |

## Build & Test

```bash
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

## Tech Stack

- C++17
- CMake 3.14+
- Google Test

## Design Notes

Each container lives in its own directory with:
- `container.h` — Header-only implementation
- `container_test.cpp` — Google Test unit tests

All containers are under the `ministl` namespace.
