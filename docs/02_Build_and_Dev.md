# Build & Development Guide

This consolidates build and developer instructions.

Prerequisites
- CMake 3.20+, Ninja, a C++20-capable compiler (MSVC or MinGW-W64), optional SFML for UI.

Standard local builds
- MinGW (Ninja):
  - mkdir build_mingw && cd build_mingw
  - cmake -G Ninja -DCMAKE_CXX_COMPILER=/path/to/g++.exe -DCMAKE_BUILD_TYPE=Release ..
  - ninja -j$(nproc)

- MSVC (via developer command prompt):
  - mkdir build && cd build
  - cmake -G "Visual Studio 18 2026" -A x64 -DCMAKE_BUILD_TYPE=Release ..
  - cmake --build . --config Release

Run tests
- ctest --output-on-failure -C Release

Developer scripts
- `scripts/build_mingw.bat` — MinGW quick build (Windows)  
- `scripts/build_release.*` — Build & optional packaging (Windows/POSIX)  
- `scripts/clean_dist.*` — Clean generated packages  
- `scripts/fix_dll_compatibility.bat`, `setup_fonts.bat` — project-specific helpers

Troubleshooting
- If SFML not found, set `-DSFML_DIR=/path/to/SFML/lib/cmake/SFML`.  
- For MinGW static runtime flags, CMake config sets `-static-libgcc -static-libstdc++` where appropriate.

Notes
- Keep build directories out of source control. Use `scripts/clean_dist.*` after packaging tests to remove generated ZIPs.


