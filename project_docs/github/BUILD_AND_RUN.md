# Build and Run Instructions

## Reversi AI - COMP390 Honours Year Project
**Student:** Tianqixing (201821852)  
**Academic Year:** 2025-26

---

## 🚀 Quick Start

### Method 1: Quick Compile Script (Recommended for Testing)

The easiest way to build and run the project:

```bash
quick_compile.bat
```

This script:
- Compiles all source files with g++ (C++20)
- Links the executable
- Automatically runs the program
- No CMake configuration needed

### Method 2: CMake Build (Recommended for Development)

```bash
# Configure and build
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run the executable
.\build\bin\reversi_ai.exe
```

### Method 3: Full Build Script

```bash
build.bat
```

---

## 📋 Prerequisites

### Required:
- **C++ Compiler** with C++20 support:
  - GCC 13.2+ (MinGW) - currently installed ✓
  - OR Visual Studio 2019/2022 with C++ tools
  
- **CMake** 3.20+ (currently installed: 4.1.2) ✓

### Optional (for future development):
- **SFML 2.5+** (for UI - Week 7)
  ```bash
  scoop install sfml
  ```
  
- **Google Test** (for unit tests - Week 3)
  ```bash
  scoop install gtest
  ```

---

## 🏗️ Build Configurations

### Debug Build (with AddressSanitizer)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Release Build (optimized)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## ✅ Current Build Status

### Successfully Compiled:
- ✓ `src/core/Board.cpp` - Bitboard implementation
- ✓ `src/core/Move.cpp` - Move representation
- ✓ `src/research/benchmark/Benchmark.cpp` - Benchmark framework
- ✓ `src/main.cpp` - Main program

### Compilation Warnings (intentional - placeholders):
- Unused parameters in placeholder functions
- Will be implemented in upcoming weeks

---

## 🐛 Troubleshooting

### Issue: CMake configuration fails
**Solution:** Use the quick compile script or check compiler installation

### Issue: "std::popcount not found"
**Solution:** Make sure C++20 standard is enabled (already configured in CMakeLists.txt)

### Issue: Missing SFML or Google Test
**Solution:** These are optional for now. Install them before Week 3 (GTest) and Week 7 (SFML)

---

## 📁 Output Structure

After building, the project structure will be:

```
build/
├── bin/
│   └── reversi_ai.exe          # Main executable
├── lib/
│   ├── libreversi_core.a       # Core library
│   └── libreversi_research.a   # Research/benchmark library
└── [CMake files]
```

---

## 🎯 Development Roadmap

### Week 1-2: Bitboard Learning (CURRENT)
- [x] Basic bitboard structure
- [ ] Legal move generation
- [ ] Bitwise move calculation

### Week 3-4: Core Engine
- [ ] Minimax with alpha-beta pruning
- [ ] Evaluation functions
- [ ] Unit tests

### Week 5-6: Advanced Features
- [ ] Move ordering
- [ ] Transposition table
- [ ] Iterative deepening

### Week 7-8: UI & Network
- [ ] SFML GUI
- [ ] Network play
- [ ] Opening book

### Week 9-11: Research & Optimization
- [ ] MCTS implementation
- [ ] Performance benchmarking
- [ ] Algorithm comparison

### Week 12: Final Testing
- [ ] Bug fixes
- [ ] Documentation
- [ ] Final report

---

## 📝 Notes

- The project uses **C++20** features like `std::popcount` and `std::countr_zero` for efficient bitboard operations
- Current implementation is a minimal framework with placeholder functions
- Main focus this week (Week 1): Understanding bitboard representation and basic operations

---

## 📚 Documentation

- [README.md](README.md) - Project overview
- [GETTING_STARTED.md](GETTING_STARTED.md) - Detailed getting started guide
- [PROJECT_SETUP_SUMMARY.md](PROJECT_SETUP_SUMMARY.md) - Complete project structure
- [docs/](docs/) - Additional documentation and research notes

---

## 🔗 Useful Commands

```bash
# Clean build
Remove-Item -Recurse -Force build
cmake -B build -G "MinGW Makefiles"
cmake --build build

# Run executable
.\build\bin\reversi_ai.exe

# OR use quick compile (no CMake needed)
quick_compile.bat
```

---

Last Updated: Week 1, Phase 1 (Bitboard Learning)

