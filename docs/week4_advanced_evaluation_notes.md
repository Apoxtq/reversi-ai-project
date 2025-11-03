# Week 4: Advanced Evaluation Function Learning Notes

**COMP390 Honours Year Project**  
**Student:** Tianqixing (201821852)  
**Week:** 4 (Advanced Position Evaluation)  
**Date:** November 3-10, 2025  
**Status:** 🔄 **IN PROGRESS** (Day 1-5 Complete, Day 6-7 Pending)

---

## 📚 Learning Objectives

1. ✅ Understand stability analysis in Reversi
2. ✅ Implement corner, edge, and frontier disc evaluation
3. ✅ Design phase-aware evaluation strategies
4. ✅ Build self-play parameter tuning framework
5. ⏳ Measure evaluation function quality improvements (pending experiments)

---

## 🎯 Week 4 Implementation Progress

### ✅ Completed Features (Day 1-5)

1. **✅ Stability Analysis System** (`src/ai/StabilityAnalyzer.hpp/cpp`)
   - ✅ Corner stability detection
   - ✅ Edge stability propagation (4 directions)
   - ✅ Frontier discs calculation (8-direction adjacent check)
   - ✅ Complete stability score computation
   - **Files:** StabilityAnalyzer.hpp (199 lines), StabilityAnalyzer.cpp (201 lines)

2. **✅ Phase-Aware Evaluation** (`src/ai/PhaseWeights.hpp`)
   - ✅ 4-phase game detection (Opening/Midgame/Endgame/Final)
   - ✅ Dynamic weight system per phase
   - ✅ Smooth weight transitions
   - **Files:** PhaseWeights.hpp (174 lines)

3. **✅ Enhanced Evaluator** (`src/ai/Evaluator_Week4.hpp/cpp`)
   - ✅ Integration with StabilityAnalyzer
   - ✅ Phase-aware weight application
   - ✅ Potential mobility calculation
   - ✅ Optimized corner control bonus
   - **Files:** Evaluator_Week4.hpp (106 lines), Evaluator_Week4.cpp (158 lines)

4. **✅ Self-Play Framework** (`src/research/self_play.cpp`)
   - ✅ AI vs AI battle system
   - ✅ Statistical data collection
   - ✅ Win rate analysis
   - **Files:** self_play.cpp (268 lines)

5. **✅ Testing Infrastructure**
   - ✅ test_stability.cpp (10+ test cases, 420 lines)
   - ✅ build_week4.bat/ps1 compilation scripts
   - ✅ All tests compile successfully

### ⏳ Pending Tasks (Day 6-7)

1. **⏳ Performance Optimization**
   - ⏳ Run stability analysis benchmarks
   - ⏳ Optimize hot paths
   - ⏳ Target: >30M evaluations/sec

2. **⏳ Experimental Validation**
   - ⏳ Run self-play experiments (Week4 vs Week3)
   - ⏳ Analyze win rates
   - ⏳ Document performance improvements

3. **⏳ Documentation Completion**
   - ⏳ Add implementation details to this document
   - ⏳ Record experimental results
   - ⏳ Final Week 4 summary

---

## 🧠 Stability Analysis Theory

### What is Stability?

**Stability** measures how "permanent" a piece is on the board:

- **Stable**: Cannot be flipped under any circumstances
- **Semi-stable**: Safe now, but could become vulnerable
- **Unstable**: Can be flipped immediately

### Types of Stability

#### 1. Corner Stability (Most Important)

```
Corners (a1, a8, h1, h8) are perfectly stable once captured.
Why? They have no directions to flip from.

Example:
X . . . . . . .    X: Stable corner
X X . . . . . .    Left X: Stable (protected by corner)
X . . . . . . .    Bottom X: Semi-stable (needs more support)
```

**Corner stability propagates**:
- If corner captured → adjacent edge pieces become stable
- Edge stability spreads along the row/column
- Creates "stable wedge" that expands into center

#### 2. Edge Stability

```
Row 0: X X X X . . . X
       ↑ ↑ ↑ ↑       ↑
       All stable if both corners captured
```

**Conditions for edge stability**:
1. Piece is on edge (row 0, 7 or col 0, 7)
2. Connected to stable corner
3. No gaps between corner and piece

#### 3. Full Stability (Complete Analysis)

**Algorithm**: BFS/DFS from corners
- Start from 4 corners
- Propagate stability along 8 directions
- Mark pieces as stable if all "attack paths" are blocked

---

## 📐 Stability Calculation Methods

### Method 1: Simple Corner-Based (Fast, ~90% accurate)

```cpp
int calculate_simple_stability(Board& board) {
    int stable_count = 0;
    
    // Check 4 corners
    if (captured_corner(a1)) {
        stable_count += count_stable_wedge(a1, board);
    }
    // ... repeat for a8, h1, h8
    
    return stable_count;
}
```

**Pros**: Very fast (< 1000 cycles)
**Cons**: Misses interior stability

### Method 2: Full Stability Analysis (Accurate, slower)

```cpp
int calculate_full_stability(Board& board) {
    uint64_t stable = 0ULL;
    
    // 1. Corner stability
    stable |= check_corner_stability(board);
    
    // 2. Edge stability (propagate from corners)
    stable |= check_edge_stability(board, stable);
    
    // 3. Interior stability (requires full lines)
    stable |= check_interior_stability(board, stable);
    
    return popcount(stable & board.player) 
         - popcount(stable & board.opponent);
}
```

**Pros**: 100% accurate
**Cons**: ~2000-3000 cycles

### Method 3: Hybrid Approach (Recommended) ✅

```cpp
int calculate_stability(Board& board, int depth) {
    if (depth >= 6) {
        // Deep search: use simple method (speed matters)
        return simple_stability(board);
    } else {
        // Shallow search: use full method (accuracy matters)
        return full_stability(board);
    }
}
```

---

## 🎲 Frontier Discs Analysis

### What are Frontier Discs?

**Frontier disc**: Your piece adjacent to at least one empty square

```
. . . X . . . .
. O O X . . . .
. O O X X . . .    X pieces on right edge are frontier
. O O O X . . .    O pieces are all frontier
. . . . X . . .
```

**Why bad?**
- Easy to flip (opponent can place adjacent)
- Decrease future options
- Give opponent mobility

### Frontier Calculation

```cpp
// Count pieces adjacent to empty squares
int count_frontier(uint64_t pieces, uint64_t empty) {
    // Shift in all 8 directions
    uint64_t adj_to_empty = 
        shift_N(empty) | shift_S(empty) |
        shift_E(empty) | shift_W(empty) |
        shift_NE(empty) | shift_NW(empty) |
        shift_SE(empty) | shift_SW(empty);
    
    return popcount(pieces & adj_to_empty);
}
```

**Penalty**: -2 points per frontier disc in opening/midgame

---

## 📊 Phase-Aware Evaluation Strategy

### Current Problem (Week 3)

```cpp
// Week 3: Simple threshold
if (total_pieces > 50) {
    score += piece_count * 10;  // Endgame
} else {
    score += piece_count;       // Opening/midgame
}
```

**Issues**:
- Only 2 phases (binary split)
- Abrupt transition at piece 50
- Doesn't reflect strategic evolution

### Week 4 Solution: Smooth Phase Transitions

```cpp
enum class GamePhase {
    Opening,    // 0-20 pieces
    EarlyMid,   // 21-35 pieces
    LateMid,    // 36-50 pieces
    Endgame     // 51-64 pieces
};

GamePhase detect_phase(int total_pieces) {
    if (total_pieces <= 20) return Opening;
    if (total_pieces <= 35) return EarlyMid;
    if (total_pieces <= 50) return LateMid;
    return Endgame;
}
```

### Phase-Specific Weights

| Feature | Opening | EarlyMid | LateMid | Endgame |
|---------|---------|----------|---------|---------|
| Piece Count | 1x | 2x | 5x | 10x |
| Mobility | 10x | 8x | 5x | 2x |
| Stability | 3x | 5x | 8x | 10x |
| Position Weights | 5x | 5x | 3x | 1x |
| Frontier Penalty | -3x | -2x | -1x | 0x |

**Rationale**:
- **Opening**: Mobility > Material (create options)
- **Early Mid**: Balance mobility and stability
- **Late Mid**: Stability becomes critical
- **Endgame**: Material is everything (pure counting)

---

## 🧪 Potential Mobility

### Definition

**Potential mobility**: Empty squares adjacent to opponent pieces

```
. . . . . . . .
. X X X O . . .    O can potentially move to 
. X O O O . . .    squares marked '*' below
. X O O . . . .
. . . . . . . .

Potential moves:
. . . * . . . .
. X X X O * . .
. X O O O * . .
. X O O * * . .
. . . * . . . .
```

**Strategic value**:
- High potential mobility → More future options
- Restricting opponent's potential mobility → Positional advantage

### Calculation

```cpp
int potential_mobility(Board& board) {
    uint64_t empty = ~(board.player | board.opponent);
    
    // Empty squares adjacent to opponent
    uint64_t player_potential = adj_to_opponent(empty, board.opponent);
    
    // Empty squares adjacent to player (opponent's potential)
    uint64_t opp_potential = adj_to_opponent(empty, board.player);
    
    return popcount(player_potential) - popcount(opp_potential);
}
```

**Weight**: 3-5x in opening/midgame, 0x in endgame

---

## 🎯 Week 4 Implementation Plan

### Day 1-2: Stability Framework

**Tasks**:
1. Implement corner detection helpers
2. Build edge stability propagation
3. Create frontier disc counter
4. Write stability unit tests

**Files**:
- `src/ai/StabilityAnalyzer.hpp` (new)
- `src/ai/StabilityAnalyzer.cpp` (new)
- `tests/test_stability.cpp` (new)

### Day 3-4: Phase-Aware Evaluator

**Tasks**:
1. Refactor Evaluator with phase detection
2. Implement smooth weight transitions
3. Add potential mobility calculation
4. Integrate stability into main evaluator

**Files**:
- `src/ai/Evaluator.hpp` (refactor)
- `src/ai/Evaluator.cpp` (refactor)
- `src/ai/PhaseWeights.hpp` (new config)

### Day 5: Self-Play Testing

**Tasks**:
1. Create self-play framework
2. Test old evaluator vs new evaluator
3. Collect win rate statistics
4. Analyze performance impact

**Files**:
- `src/research/self_play.cpp` (new)
- `docs/week4_experiments.md` (new)

### Day 6-7: Optimization + Documentation

**Tasks**:
1. Profile evaluation function performance
2. Optimize hot paths
3. Write comprehensive documentation
4. Finalize Week 4 notes

---

## 📈 Success Metrics

### Performance Targets

| Metric | Target | Stretch Goal |
|--------|--------|--------------|
| Eval speed | > 30M evals/sec | > 40M evals/sec |
| Stability calc | < 100 cycles | < 50 cycles |
| Code quality | 100% tests pass | 0 warnings |

### AI Strength Targets

| Matchup | Target Win Rate | Stretch Goal |
|---------|-----------------|--------------|
| New vs Old (depth 5) | > 60% | > 70% |
| New vs Random | > 95% | 100% |
| New depth 4 vs Old depth 5 | > 40% | > 50% |

---

## 🔬 Experimental Questions

1. **Stability vs Mobility**: Which matters more in midgame?
2. **Frontier Penalty**: What's the optimal weight?
3. **Phase Transitions**: Are 4 phases better than 3?
4. **Performance Cost**: Does stability analysis slow search?

---

## 📚 References

1. **Logistello** - World champion Reversi program (1997)
   - Stability analysis techniques
   - Phase-dependent evaluation

2. **Edax** - Modern open-source Reversi engine
   - Efficient stability calculation
   - Bitboard tricks

3. **University Research Papers**:
   - Lee & Mahajan (1990): "The Development of a World Class Othello Program"
   - Buro (1997): "Experiments with Multi-ProbCut and a New High-Quality Evaluation Function for Othello"

---

## ✅ Deliverables Checklist

- [ ] StabilityAnalyzer.hpp/cpp implemented
- [ ] PhaseWeights.hpp configuration created
- [ ] Evaluator.cpp refactored with new features
- [ ] test_stability.cpp with 10+ test cases
- [ ] self_play.cpp functional
- [ ] Performance benchmarks run
- [ ] Week 4 notes completed (this file)
- [ ] Experimental data documented

---

**Last Updated**: 2025-11-03  
**Next Milestone**: Week 5 - Transposition Tables


