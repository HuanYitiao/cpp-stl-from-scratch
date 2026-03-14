# stl-from-scratch

A from-scratch implementation of C++ STL containers, built for deep understanding of data structures and memory management.

## Roadmap

### Phase 1 — Sequence Containers
| # | Container | Status |
|---|-----------|--------|
| 1 | `vector` | 🔨 In Progress |
| 2 | `list` | ⬜ |

### Phase 2 — Adapters + Deque
| # | Container | Status |
|---|-----------|--------|
| 3 | `deque` | ⬜ |
| 4 | `stack` | ⬜ |
| 5 | `queue` | ⬜ |

### Phase 3 — Hash Table Family
| # | Container | Status |
|---|-----------|--------|
| 6 | `hashtable` (engine) | ⬜ |
| 7 | `unordered_set` | ⬜ |
| 8 | `unordered_map` | ⬜ |

### Phase 4 — Red-Black Tree Family
| # | Container | Status |
|---|-----------|--------|
| 9 | `rbtree` (engine) | ⬜ |
| 10 | `set` | ⬜ |
| 11 | `map` | ⬜ |
| 12 | `multiset` | ⬜ |
| 13 | `multimap` | ⬜ |

### Phase 5 — Heap Adapter
| # | Container | Status |
|---|-----------|--------|
| 14 | `priority_queue` | ⬜ |

## Build & Test

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

## Tech Stack

- C++17
- CMake 3.14+
- Google Test (fetched automatically)
- GitHub Actions CI (GCC + Clang)

## Project Structure

```
include/ministl/     Header-only implementations
tests/               Google Test unit tests
.github/workflows/   CI configuration
```

All containers live under the `ministl` namespace.
