# Week 5: Transposition Table Optimization Learning Notes

**COMP390 Honours Year Project**  
**Student:** Tianqixing (201821852)  
**Week:** 5 (Transposition Table & Optimization)  
**Date:** November 10-17, 2025  
**Status:** 🔄 **IN PROGRESS**

---

## 📚 Learning Objectives

1. ⏳ Understand transposition table theory and implementation
2. ⏳ Design efficient hash table structure for game positions
3. ⏳ Implement replacement strategies (depth-preferred vs always-replace)
4. ⏳ Integrate transposition table with Minimax search engine
5. ⏳ Optimize move ordering using transposition table entries
6. ⏳ Measure performance improvements (cache hit rate, search speed)

---

## 🎯 Week 5 Implementation Progress

### ⏳ Planned Features

1. **⏳ Transposition Table Data Structure** (`src/ai/TranspositionTable.hpp/cpp`)
   - ⏳ TTEntry structure (hash, score, depth, flag, best_move)
   - ⏳ Hash table with configurable size (2^N entries)
   - ⏳ Memory alignment optimization
   - ⏳ Hash collision detection

2. **⏳ Replacement Strategies**
   - ⏳ Depth-preferred replacement (keep deeper entries)
   - ⏳ Always-replace strategy (simple, fast)
   - ⏳ Performance comparison

3. **⏳ MinimaxEngine Integration**
   - ⏳ Query transposition table before search
   - ⏳ Store search results after evaluation
   - ⏳ Handle Alpha-Beta bounds (EXACT/LOWER/UPPER)
   - ⏳ Depth validation

4. **⏳ Move Ordering Optimization**
   - ⏳ Prioritize transposition table best moves
   - ⏳ Combine with existing evaluation-based ordering
   - ⏳ Measure pruning efficiency improvement

5. **⏳ Performance Benchmarking**
   - ⏳ Cache hit rate statistics
   - ⏳ Search speed comparison (with/without TT)
   - ⏳ Memory usage monitoring
   - ⏳ Target: >40% hit rate, >3M nodes/sec

6. **⏳ Testing Infrastructure**
   - ⏳ test_transposition.cpp (unit tests)
   - ⏳ bench_week5.cpp (performance benchmarks)
   - ⏳ Hash collision tests

---

## 🧠 Transposition Table Theory

### What is a Transposition Table?

A **transposition table** (TT) is a cache that stores previously evaluated game positions to avoid redundant calculations.

**Key Concept:**
- Same position can be reached through different move sequences (transpositions)
- Example: Move sequence A→B→C reaches same position as C→A→B
- Instead of re-searching, we can reuse cached results

### Why Use Transposition Tables?

1. **Eliminate Redundant Work**
   - Avoid re-searching identical positions
   - Especially effective in iterative deepening

2. **Improve Move Ordering**
   - Cached best moves guide search
   - Better move ordering → more alpha-beta pruning

3. **Performance Boost**
   - Typical improvement: 2-5x search speed
   - Cache hit rates: 30-60% in practice

### Transposition Table Entry Structure

```cpp
struct TTEntry {
    uint64_t hash;        // 64-bit Zobrist hash (for verification)
    int16_t score;        // Evaluation score
    int8_t depth;         // Search depth (for depth validation)
    uint8_t flag;         // Entry type: EXACT, LOWER_BOUND, UPPER_BOUND
    int8_t best_move;     // Best move found (0-63, or -1)
    // Total: 16 bytes (aligned for cache efficiency)
};
```

**Entry Types (Flags):**
- **EXACT**: Score is exact (alpha < score < beta)
- **LOWER_BOUND**: Score is lower bound (score >= beta, beta-cutoff)
- **UPPER_BOUND**: Score is upper bound (score <= alpha, alpha-cutoff)

### Hash Table Design

**Size Selection:**
- Use power-of-2 sizes for fast modulo: `index = hash & (size - 1)`
- Typical sizes: 2^20 (1M entries, ~16 MB) to 2^22 (4M entries, ~64 MB)
- Balance: More entries = better hit rate, but more memory

**Collision Handling:**
- **Simple replacement**: Overwrite existing entry (fast, may lose data)
- **Depth-preferred**: Keep entry with greater depth (better quality)
- **Always-replace**: Always overwrite (simple, works well in practice)

### Integration with Alpha-Beta

**Query Before Search:**
```cpp
TTEntry* entry = tt.probe(hash);
if (entry && entry->depth >= depth) {
    if (entry->flag == EXACT) {
        return entry->score;  // Exact match
    }
    if (entry->flag == LOWER_BOUND && entry->score >= beta) {
        return beta;  // Beta cutoff
    }
    if (entry->flag == UPPER_BOUND && entry->score <= alpha) {
        return alpha;  // Alpha cutoff
    }
}
```

**Store After Search:**
```cpp
TTEntry new_entry;
new_entry.hash = hash;
new_entry.score = score;
new_entry.depth = depth;
if (score <= alpha) {
    new_entry.flag = UPPER_BOUND;
} else if (score >= beta) {
    new_entry.flag = LOWER_BOUND;
} else {
    new_entry.flag = EXACT;
}
new_entry.best_move = best_move;
tt.store(new_entry);
```

---

## 🔧 Implementation Details

### TranspositionTable Class Design

```cpp
class TranspositionTable {
public:
    enum Flag {
        EXACT = 0,      // Exact score
        LOWER_BOUND = 1, // Beta cutoff (score >= beta)
        UPPER_BOUND = 2  // Alpha cutoff (score <= alpha)
    };
    
    struct Entry {
        uint64_t hash;      // 64-bit hash (for verification)
        int16_t score;      // Evaluation score
        int8_t depth;       // Search depth
        uint8_t flag;       // Entry type
        int8_t best_move;   // Best move (0-63, or -1)
        // Total: 16 bytes (cache-aligned)
    };
    
    // Constructor: allocate table with 2^size_bits entries
    explicit TranspositionTable(int size_bits = 20);
    
    // Query entry for given hash
    Entry* probe(uint64_t hash);
    
    // Store entry (with replacement strategy)
    void store(const Entry& entry);
    
    // Clear all entries
    void clear();
    
    // Statistics
    size_t size() const { return size_; }
    size_t entries() const { return entries_; }
    double hit_rate() const { return hit_rate_; }
    
private:
    std::vector<Entry> table_;  // Hash table
    size_t size_;               // Table size (2^size_bits)
    size_t size_mask_;          // Mask for fast modulo (size - 1)
    size_t entries_;            // Number of stored entries
    size_t hits_;               // Cache hits
    size_t misses_;             // Cache misses
    double hit_rate_;           // Hit rate (hits / (hits + misses))
};
```

### Replacement Strategy: Depth-Preferred

```cpp
void TranspositionTable::store(const Entry& entry) {
    size_t index = entry.hash & size_mask_;
    Entry& existing = table_[index];
    
    // Replacement strategy: depth-preferred
    if (existing.hash == 0 ||  // Empty slot
        existing.depth < entry.depth ||  // Deeper entry
        (existing.depth == entry.depth && existing.hash != entry.hash)) {
        // Replace if deeper, or same depth but different position
        existing = entry;
        ++entries_;
    }
}
```

### MinimaxEngine Integration

**Before Search:**
```cpp
int MinimaxEngine::negamax(Board& board, int depth, int alpha, int beta) {
    // Query transposition table
    uint64_t hash = board.get_hash();
    TTEntry* entry = tt_.probe(hash);
    
    if (entry && entry->depth >= depth) {
        // Use cached result if depth is sufficient
        if (entry->flag == TTEntry::EXACT) {
            return entry->score;
        }
        if (entry->flag == TTEntry::LOWER_BOUND && entry->score >= beta) {
            return beta;  // Beta cutoff
        }
        if (entry->flag == TTEntry::UPPER_BOUND && entry->score <= alpha) {
            return alpha;  // Alpha cutoff
        }
    }
    
    // Continue with normal search...
}
```

**After Search:**
```cpp
    // Store result in transposition table
    TTEntry new_entry;
    new_entry.hash = hash;
    new_entry.score = best_score;
    new_entry.depth = depth;
    new_entry.best_move = best_move;
    
    if (best_score <= alpha) {
        new_entry.flag = TTEntry::UPPER_BOUND;
    } else if (best_score >= beta) {
        new_entry.flag = TTEntry::LOWER_BOUND;
    } else {
        new_entry.flag = TTEntry::EXACT;
    }
    
    tt_.store(new_entry);
    
    return best_score;
}
```

### Move Ordering with Transposition Table

```cpp
std::vector<int> order_moves(const Board& board, uint64_t hash) {
    auto moves = board.get_legal_moves();
    
    // Query transposition table for best move
    TTEntry* entry = tt_.probe(hash);
    int tt_move = (entry && entry->best_move >= 0) ? entry->best_move : -1;
    
    // Sort moves: TT move first, then by evaluation
    std::sort(moves.begin(), moves.end(), [&](int a, int b) {
        // TT move has highest priority
        if (a == tt_move) return true;
        if (b == tt_move) return false;
        
        // Then sort by evaluation (higher is better)
        return evaluate_move(board, a) > evaluate_move(board, b);
    });
    
    return moves;
}
```

---

## 📊 Performance Targets

| Metric | Current (Week 3) | Target (Week 5) | Improvement |
|--------|------------------|------------------|-------------|
| **Search Speed** | 1.91 M nodes/sec | >3 M nodes/sec | ~57% |
| **Cache Hit Rate** | N/A | >40% | New |
| **Depth 6 Time** | 1.14 ms | <0.8 ms | ~30% |
| **Memory Usage** | ~0 MB | ~16-64 MB | New |
| **Alpha-Beta Efficiency** | 67.5% | >70% | ~4% |

---

## 🧪 Testing Strategy

### Unit Tests

1. **Basic Storage/Retrieval**
   - Store entry and retrieve it
   - Verify hash collision detection
   - Test replacement strategies

2. **Depth Validation**
   - Only use entries with sufficient depth
   - Reject entries with depth < current depth

3. **Flag Handling**
   - Test EXACT, LOWER_BOUND, UPPER_BOUND flags
   - Verify correct alpha-beta integration

4. **Hash Collision**
   - Test collision detection (different positions, same hash)
   - Verify 64-bit hash collision rate is negligible

### Performance Benchmarks

1. **Cache Hit Rate**
   - Measure hit rate during typical game search
   - Compare different table sizes

2. **Search Speed**
   - Compare search speed with/without TT
   - Measure speedup factor

3. **Memory Usage**
   - Monitor memory consumption
   - Verify table size calculations

---

## 📝 Implementation Checklist

- [ ] Create `TranspositionTable.hpp` (class definition)
- [ ] Create `TranspositionTable.cpp` (implementation)
- [ ] Implement basic storage/retrieval
- [ ] Implement replacement strategies
- [ ] Add hash collision detection
- [ ] Integrate with `MinimaxEngine`
- [ ] Implement move ordering optimization
- [ ] Create unit tests (`test_transposition.cpp`)
- [ ] Create performance benchmarks (`bench_week5.cpp`)
- [ ] Document performance improvements
- [ ] Update `PROJECT_PLAN.md`

---

## 🎓 Key Learnings

### Transposition Table Best Practices

1. **Hash Size**: Use 64-bit Zobrist hashes (collision rate ~0)
2. **Table Size**: 2^20 to 2^22 entries (balance hit rate vs memory)
3. **Replacement**: Depth-preferred works well in practice
4. **Memory**: Align entries to cache line size (16 bytes)
5. **Validation**: Always check hash match (collision detection)

### Common Pitfalls

1. **Depth Mismatch**: Don't use entries with insufficient depth
2. **Flag Confusion**: Correctly handle EXACT/LOWER/UPPER bounds
3. **Hash Collision**: Always verify hash match before using entry
4. **Memory Overflow**: Monitor table size and memory usage
5. **Thread Safety**: Single-threaded for now, consider locks later

---

## 📚 References

1. **Zobrist Hashing**: Efficient hash function for game positions
2. **Alpha-Beta Pruning**: Integration with transposition tables
3. **Iterative Deepening**: Works well with transposition tables
4. **Move Ordering**: Using TT entries to improve search efficiency

---

**Last Updated:** November 10, 2025  
**Status:** 🔄 In Progress

