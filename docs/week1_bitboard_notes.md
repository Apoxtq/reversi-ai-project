# Week 1: Bitboard Implementation Learning Notes

**COMP390 Honours Year Project**  
**Student:** Tianqixing (201821852)  
**Week:** 1 (Bitboard Learning Phase)  
**Date:** October 2025

---

## 📚 Learning Objectives

1. Understand bitboard representation for game boards
2. Learn bitwise operations for move generation
3. Implement efficient board state manipulation
4. Master C++20 bit manipulation features

---

## 🎯 Bitboard Basics

### What is a Bitboard?

A bitboard is a 64-bit integer where each bit represents a square on the 8x8 Reversi board:

```
Bit positions (0-63):
 0  1  2  3  4  5  6  7    (Row 0)
 8  9 10 11 12 13 14 15    (Row 1)
16 17 18 19 20 21 22 23    (Row 2)
24 25 26 27 28 29 30 31    (Row 3)
32 33 34 35 36 37 38 39    (Row 4)
40 41 42 43 44 45 46 47    (Row 5)
48 49 50 51 52 53 54 55    (Row 6)
56 57 58 59 60 61 62 63    (Row 7)

Reversi board notation:
  A  B  C  D  E  F  G  H
8 . . . . . . . .  8
7 . . . . . . . .  7
6 . . . . . . . .  6
5 . . . O X . . .  5
4 . . . X O . . .  4
3 . . . . . . . .  3
2 . . . . . . . .  2
1 . . . . . . . .  1
  A  B  C  D  E  F  G  H
```

### Initial Position

In Reversi, the starting position is:
- White (O): d4 (27), e5 (36)
- Black (X): d5 (35), e4 (28)

In bitboard representation:
```cpp
uint64_t white = 0x0000000810000000ULL;   // Bits 27, 36 set
uint64_t black = 0x0000001008000000ULL;   // Bits 28, 35 set
```

---

## 🔧 C++20 Bit Manipulation Functions

### Key Functions Used

| Function | Purpose | Example |
|----------|---------|---------|
| `std::popcount(x)` | Count set bits | `popcount(0b1011) = 3` |
| `std::countr_zero(x)` | Count trailing zeros | `countr_zero(0b1000) = 3` |
| `std::countl_zero(x)` | Count leading zeros | `countl_zero(0b0010) = 61` |
| `std::has_single_bit(x)` | Check if power of 2 | `has_single_bit(8) = true` |

### Useful Bit Tricks

```cpp
// Set bit at position
uint64_t setBit(uint64_t board, int pos) {
    return board | (1ULL << pos);
}

// Clear bit at position
uint64_t clearBit(uint64_t board, int pos) {
    return board & ~(1ULL << pos);
}

// Toggle bit at position
uint64_t toggleBit(uint64_t board, int pos) {
    return board ^ (1ULL << pos);
}

// Check if bit is set
bool testBit(uint64_t board, int pos) {
    return (board & (1ULL << pos)) != 0;
}

// Get least significant set bit
uint64_t getLSB(uint64_t board) {
    return board & -board;
}

// Clear least significant bit
uint64_t clearLSB(uint64_t board) {
    return board & (board - 1);
}
```

---

## 🎮 Legal Move Generation Algorithm

### Direction Vectors for Reversi

Reversi has 8 directions to check for captures:

```cpp
const int DIRECTIONS[8] = {
    -9, -8, -7,  // NW, N, NE
    -1,     +1,  // W,     E
    +7, +8, +9   // SW, S, SE
};

// With edge masking:
const uint64_t NOT_A_FILE = 0xfefefefefefefefeULL; // ~A file
const uint64_t NOT_H_FILE = 0x7f7f7f7f7f7f7f7fULL; // ~H file
```

### Basic Algorithm (to be implemented)

```cpp
uint64_t legalMoves(uint64_t player, uint64_t opponent) {
    uint64_t legal = 0;
    uint64_t empty = ~(player | opponent);
    
    // For each direction
    for (int dir : DIRECTIONS) {
        uint64_t candidates = shift(player, dir) & opponent;
        
        // Continue in direction while hitting opponent pieces
        while (candidates) {
            uint64_t next = shift(candidates, dir);
            legal |= next & empty;
            candidates = next & opponent;
        }
    }
    
    return legal;
}
```

---

## 📝 Implementation Tasks

### ✅ Completed
- [x] Basic Board structure with bitboards
- [x] Initial position setup
- [x] Piece counting with `std::popcount`
- [x] Board display/printing
- [x] Project compilation with C++20

### 🔄 In Progress
- [ ] Legal move generation
- [ ] Bitwise shift operations for directions
- [ ] Edge case handling (boundaries)

### 📅 To Do (Week 2)
- [ ] Move execution (flipping pieces)
- [ ] Undo move functionality
- [ ] Terminal state detection
- [ ] Zobrist hashing for positions

---

## 🐛 Challenges and Solutions

### Challenge 1: C++ Standard Version
**Problem:** Initially used C++17, but `std::popcount` requires C++20

**Solution:** Updated CMakeLists.txt:
```cmake
set(CMAKE_CXX_STANDARD 20)
```

### Challenge 2: CMake Output Issues
**Problem:** CMake not producing visible output in PowerShell

**Solution:** Created `quick_compile.bat` script for direct g++ compilation

### Challenge 3: Understanding Bit Positions
**Problem:** Confusion between chess notation and bit indices

**Solution:** Created mapping:
```cpp
// Convert algebraic notation (e.g., "e4") to bit index
int algebraicToBit(const string& pos) {
    int col = pos[0] - 'a';  // 0-7
    int row = '8' - pos[1];  // 0-7
    return row * 8 + col;
}
```

---

## 📚 References and Resources

### Key Resources Used
1. **Chess Programming Wiki** - Bitboards
   - https://www.chessprogramming.org/Bitboards
   
2. **Reversi/Othello Bitboard Tutorial**
   - Algorithm explanations and optimizations

3. **C++20 Bit Manipulation** - cppreference
   - https://en.cppreference.com/w/cpp/header/bit

4. **GCC Intrinsics Documentation**
   - Built-in functions like `__builtin_popcountll`

### Recommended Reading
- "Bitboard Methods for Games" - academic paper
- Edax (Othello engine) source code analysis
- Modern C++ bitwise optimization techniques

---

## 🔬 Experiments and Tests

### Experiment 1: Performance of std::popcount
```cpp
// Test: Count pieces on full board
uint64_t board = 0xFFFFFFFFFFFFFFFFULL;

// Method 1: std::popcount (C++20)
int count1 = std::popcount(board);  // ~1-2 cycles (hardware)

// Method 2: Brian Kernighan's algorithm
int count2 = 0;
while (board) {
    count2++;
    board &= board - 1;  // ~64 iterations worst case
}

// Result: std::popcount is 10-30x faster!
```

### Experiment 2: Bit Iteration
```cpp
// Extract all set bit positions
std::vector<int> setBits(uint64_t board) {
    std::vector<int> positions;
    while (board) {
        int pos = std::countr_zero(board);
        positions.push_back(pos);
        board &= board - 1;  // Clear LSB
    }
    return positions;
}
```

---

## 💡 Key Takeaways

1. **Bitboards are extremely efficient** for board representation
   - Single 64-bit integer vs. 64-element array
   - Parallel operations on all squares
   
2. **C++20 provides excellent bit manipulation support**
   - Hardware-accelerated functions
   - Cleaner, more readable code than raw bitwise ops
   
3. **Edge handling is critical**
   - Must use masks to prevent wraparound
   - NOT_A_FILE, NOT_H_FILE masks essential
   
4. **Shift operations are direction-specific**
   - Each direction has different shift amount
   - Must handle edge cases per direction

---

## 📊 Next Week Preview

### Week 2 Goals
1. Complete legal move generation
2. Implement move execution with flipping
3. Add board state validation
4. Start Zobrist hashing for transposition table

### Preparation Tasks
- Read about Zobrist hashing algorithm
- Study advanced bitboard patterns
- Review minimax algorithm basics

---

## 🤔 Questions for Further Research

1. Can we optimize legal move generation further?
   - SIMD instructions?
   - Lookup tables?

2. What's the best way to handle move history?
   - Stack-based undo?
   - Copy-make vs incremental updates?

3. How do professional engines handle bitboards?
   - Study Edax, NTest, Zebra engines

---

**Status:** Week 1 - In Progress  
**Next Review:** End of Week 1  
**Last Updated:** October 8, 2025

