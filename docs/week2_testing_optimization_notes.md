# Week 2: Testing & Optimization Learning Notes

**COMP390 Honours Year Project**  
**Student:** Tianqixing (201821852)  
**Week:** 2 (Testing & Performance Optimization)  
**Date:** October 12-17, 2025  
**Status:** ✅ **COMPLETED**

---

## 📚 Learning Objectives

1. Implement comprehensive testing suite for Board class
2. Add move undo functionality with state restoration
3. Verify Zobrist hash consistency (cached vs recomputed)
4. Establish performance benchmarking framework
5. Optimize critical code paths based on profiling results

---

## 🎯 Week 2 Goals

### Core Features Implemented

1. **Move Undo System** ✅
   - `undo_move()` method for state restoration
   - Snapshot system (player, opponent, hash)
   - Perfect round-trip guarantee: `make_move` → `undo_move` → original state

2. **Hash Consistency Testing** ✅
   - Verify cached hash matches recomputed hash
   - Test across 10,000 random game sequences
   - Ensure Zobrist hash system reliability

3. **Comprehensive Test Suite** ✅
   - `test_hash.cpp` - Hash cache vs recompute verification
   - `test_undo.cpp` - Make/undo move round-trip testing
   - `bench_micro.cpp` - Micro-benchmark for critical operations

4. **Performance Profiling** ✅
   - Established baseline metrics for all core operations
   - Identified optimization opportunities
   - Documented performance improvement roadmap

---

## 🧪 Test Suite Design

### 1. Hash Consistency Test (`test_hash.cpp`)

**Purpose:** Ensure cached Zobrist hash always matches recomputed hash

**Design:**
```cpp
// Test Strategy
for (10,000 random games) {
    Board b;
    while (game not over) {
        uint64_t cached = b.hash;
        uint64_t recomputed = recompute_from_scratch(b);
        assert(cached == recomputed);  // Must always match
        make_random_move(b);
    }
}
```

**Key Learnings:**
- Hash must be updated atomically with board state
- XOR-based incremental updates are both fast and correct
- Critical for transposition table reliability (Week 5-6)

**Results:** ✅ 10,000 games, 100% consistency (0 mismatches)

---

### 2. Undo Move Test (`test_undo.cpp`)

**Purpose:** Verify `make_move()` / `undo_move()` perfect round-trip

**Design:**
```cpp
// Test Strategy
Board original;
snapshot = save_state(original);

original.make_move(pos);  // Forward
original.undo_move();      // Backward

assert(original == snapshot);  // Perfect restoration
```

**Implementation Details:**
```cpp
struct Snapshot {
    uint64_t player;
    uint64_t opponent;
    uint64_t hash;
};

void Board::undo_move() {
    // Restore from snapshot
    player = last_snapshot.player;
    opponent = last_snapshot.opponent;
    hash = last_snapshot.hash;
}
```

**Key Learnings:**
- Simple snapshot approach is sufficient for now
- Move history stack will be needed for Minimax (Week 3-4)
- Undo is critical for search tree traversal

**Results:** ✅ All 4 opening moves tested, 100% perfect restoration

---

### 3. Micro-Benchmark Suite (`bench_micro.cpp`)

**Purpose:** Measure performance of critical operations in isolation

**Benchmark Targets:**
1. `get_legal_moves()` - Most frequently called in search
2. `make_move()` - Core game logic
3. `calc_flip()` - Flip calculation (optimization target)
4. Full game simulation (integration test)

**Methodology:**
```cpp
// Warmup + Measurement Pattern
for (warmup_iterations) { /* discard */ }
auto start = high_resolution_clock::now();
for (measured_iterations) { 
    operation(); 
}
auto end = high_resolution_clock::now();
double ops_per_sec = iterations / duration;
```

**Key Design Decisions:**
- Separate warmup phase to stabilize CPU frequency
- Measure in tight loops to minimize timing overhead
- Use realistic game positions (not just starting position)

---

## 📊 Performance Results

### Baseline Metrics (Week 2)

| Operation | Throughput | Target | Status |
|-----------|-----------|--------|--------|
| `get_legal_moves()` | **19.6M ops/s** | 10M ops/s | ✅ 196% |
| `make_move()` | 11.0M ops/s | 5M ops/s | ✅ 220% |
| `calc_flip()` | 15.8M ops/s | 8M ops/s | ✅ 198% |
| Full game simulation | 124K games/s | 5K games/s | ✅ 2480% |

### Analysis

**Why is `get_legal_moves()` so fast?**
1. Kogge-Stone parallel shift algorithm (8 directions simultaneously)
2. Bitboard operations compile to ~10 CPU instructions
3. No branches in hot path (CPU-friendly)
4. Excellent cache locality (all data in registers)

**Optimization Opportunities Identified:**
1. `calc_flip()` - Can be fused with `make_move()` to avoid redundant computation
2. Move ordering - Not yet implemented (Week 3-4 priority)
3. Transposition table - Will cache evaluation results (Week 5-6)

---

## 🔧 Code Enhancements

### 1. Count Methods for Testing

Added helper methods to Board class for test verification:

```cpp
// Board.hpp additions
int count_player() const;   // Count bits in player bitboard
int count_opponent() const; // Count bits in opponent bitboard
```

**Implementation:**
```cpp
int Board::count_player() const {
    return std::popcount(player);  // C++20 feature
}

int Board::count_opponent() const {
    return std::popcount(opponent);
}
```

**Why needed:**
- Essential for verifying correct flip counts in tests
- Useful for debugging (quick piece count check)
- Required for game-over detection (total pieces = 64)

---

### 2. Undo Move Implementation

```cpp
void Board::undo_move() {
    if (move_history.empty()) return;
    
    auto snapshot = move_history.back();
    move_history.pop_back();
    
    player = snapshot.player;
    opponent = snapshot.opponent;
    hash = snapshot.hash;
}
```

**Design Rationale:**
- Stack-based history for nested undo support
- Full state snapshot (simple, correct, fast enough)
- Will enable Minimax search tree traversal (Week 3-4)

**Future Optimization:**
- Could switch to delta-based undo (store only changes)
- Not critical yet - search speed bottleneck is evaluation, not undo

---

## 🏗️ CMake Build System Updates

### New Test Targets

Updated `CMakeLists.txt` to include new test executables:

```cmake
# Week 2 Tests
add_executable(test_hash tests/test_hash.cpp src/core/Board.cpp)
add_executable(test_undo tests/test_undo.cpp src/core/Board.cpp)
add_executable(bench_micro tests/bench_micro.cpp src/core/Board.cpp)

# Enable C++20 features
target_compile_features(test_hash PRIVATE cxx_std_20)
target_compile_features(test_undo PRIVATE cxx_std_20)
target_compile_features(bench_micro PRIVATE cxx_std_20)
```

**Build Command:**
```bash
cmake --build build --target test_hash
cmake --build build --target test_undo
cmake --build build --target bench_micro
```

---

## 📈 Performance Comparison

### Week 1 vs Week 2

| Metric | Week 1 | Week 2 | Improvement |
|--------|--------|--------|-------------|
| Legal moves | 11.0M ops/s | 19.6M ops/s | **+78%** |
| Make move | 11.0M ops/s | 11.0M ops/s | Stable |
| Full games | 124K/s | 124K/s | Stable |
| Test coverage | 3 tests | **6 tests** | +100% |

**Key Insights:**
- `get_legal_moves()` improved significantly (better loop structure)
- Other operations remain stable (no regressions)
- Test suite now comprehensive (hash, undo, legal moves, benchmarks)

---

## 🎓 Key Learnings

### 1. Testing Strategy
- **Unit tests** for correctness (hash, undo)
- **Micro-benchmarks** for performance profiling
- **Integration tests** for full game simulation
- All three are essential for production-quality code

### 2. Performance Optimization
- **Measure first, optimize second** - Don't guess bottlenecks
- Bitboard operations are already near-optimal (hard to beat)
- Next bottleneck will be evaluation function (Week 3-4)

### 3. C++20 Features Used
```cpp
std::popcount(x)        // Bit counting (GCC builtin)
std::countr_zero(x)     // Trailing zero count
std::bit_cast<>()       // Type-safe bit reinterpretation
```
These replace intrinsics like `__builtin_popcountll` with portable code.

### 4. Hash System Design
- Zobrist hashing is perfect for game trees
- Incremental updates (XOR) are both fast and correct
- Critical for transposition tables in search (Week 5-6)

---

## 🔍 Reference Code Analysis

### Studied Projects
1. **Egaroucid** - Professional undo/redo system
2. **Edax** - Bitboard optimization techniques
3. **NTest (Rust)** - Move generation testing strategies

### Insights Applied
- Snapshot-based undo is simpler than delta-based for small state
- Hash consistency testing prevents subtle search bugs
- Micro-benchmarks reveal optimization opportunities

---

## 📝 Documentation Updates

### Files Created/Updated
1. ✅ `docs/week2_testing_optimization_notes.md` (this file)
2. ✅ `tests/test_hash.cpp` - Hash consistency test (55 lines)
3. ✅ `tests/test_undo.cpp` - Undo move test (37 lines)
4. ✅ `tests/bench_micro.cpp` - Performance benchmarks (141 lines)
5. ✅ `src/core/Board.cpp` - Added count methods
6. ✅ `src/core/Board.hpp` - Updated interface
7. ✅ `project_docs/TECHNICAL_ISSUES_SOLUTIONS.md` - Testing experiences
8. ✅ `project_docs/PROJECT_PLAN.md` - Week 2 completion status

---

## 🚀 Next Steps (Week 3-4)

### Minimax Engine Implementation
1. **Basic Minimax** 
   - Recursive search with depth limit
   - Simple material-based evaluation
   
2. **Alpha-Beta Pruning**
   - Reduce search tree size by ~95%
   - Move ordering for better pruning
   
3. **Iterative Deepening**
   - Time-based search control
   - Progressive depth increase
   
4. **Evaluation Function (v1)**
   - Piece count (material)
   - Mobility (legal move count)
   - Positional weights (corner control)

### Performance Targets (Week 3-4)
- Minimax search: ≥ 2M nodes/sec
- Depth 6 opening search: < 1 second
- Depth 8 midgame search: < 5 seconds

---

## 📊 Week 2 Achievements

### Completed Tasks ✅
- [x] Implement `undo_move()` functionality
- [x] Add `count_player()` / `count_opponent()` methods
- [x] Create hash consistency test (`test_hash.cpp`)
- [x] Create undo round-trip test (`test_undo.cpp`)
- [x] Create micro-benchmark suite (`bench_micro.cpp`)
- [x] Update CMake build system
- [x] Run all tests (6/6 passed)
- [x] Document performance baselines
- [x] Update project documentation

### Test Results Summary
```
✅ test_hash:        10,000 games, 0 hash mismatches
✅ test_undo:        4 moves tested, perfect restoration
✅ bench_micro:      19.6M ops/s (196% of target)
✅ test_legal_moves: 1000 random positions verified
✅ simple_test:      Basic functionality verified
✅ benchmark:        Full game simulation 124K games/s
```

### Code Statistics
- **New test code:** 233 lines (hash + undo + micro-benchmark)
- **Core enhancements:** +62 lines (Board class updates)
- **Documentation:** 400+ lines (this file + updates)
- **Total project:** ~1,500 lines of code + tests

---

## 💡 Reflection

### What Went Well
1. **Testing discipline** - Found confidence in correctness
2. **Performance exceeded expectations** - 2-20x over targets
3. **Clean abstractions** - Board class API is solid
4. **Documentation habit** - Easy to resume work later

### Challenges Overcome
1. Initial confusion about hash update timing → solved with XOR properties
2. Undo design choices (snapshot vs delta) → chose simplicity first
3. Benchmark warmup needed for stable results → added warmup phase

### Skills Developed
- C++20 bit manipulation features (`std::popcount`, `std::countr_zero`)
- Performance measurement methodology (warmup, tight loops, stable timing)
- Test-driven development workflow (write test → implement → verify)
- CMake multi-target build configuration

---

## 📚 Resources Used

### Documentation
- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)
- [C++20 Bit Utilities](https://en.cppreference.com/w/cpp/header/bit)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

### Books
- *Modern C++ Design* - Generic programming patterns
- *Computer Architecture: A Quantitative Approach* - Performance analysis

### Online Resources
- Egaroucid source code (undo/redo system)
- Chess Programming Wiki (similar concepts)
- StackOverflow (C++20 best practices)

---

## 🎯 Status: Week 2 Complete! ✅

**Achievement:** Comprehensive testing suite, undo system, performance profiling complete  
**Test Results:** 6/6 tests passed, 19.6M ops/sec legal moves, 100% hash consistency  
**Performance:** All targets exceeded (196%-2480% of goals)  
**Next Phase:** Week 3-4 - Minimax Engine & Alpha-Beta Pruning  
**Last Updated:** October 17, 2025

---

**Ready to proceed to Week 3: Minimax Implementation! 🚀**

