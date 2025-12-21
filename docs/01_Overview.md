# ReversiFinalProj — Overview

This document consolidates high-level project information and layout.

Project layout (canonical)

- `src/` — C++ source code
  - `core/` — core game logic
  - `ai/` — Minimax, MCTS, Transposition table
  - `ui/` — SFML UI components
  - `network/` — networking code
  - `research/` — benchmarks and experiments

- `tests/` — unit tests and benchmarks  
- `scripts/` — helper scripts (build, package, clean)  
- `dist/` — packaging output (ignored)  
- `build*/` — build directories (ignored)

Files of interest:
- `CMakeLists.txt` — build configuration  
- `docs/01_Overview.md` — this file (project overview)  
- `docs/02_Build_and_Dev.md` — build & developer guide  
- `docs/03_Packaging_and_Release.md` — packaging, release checklist, changelog

Notes:
- Weekly development logs `week1`..`week11` in `project_docs/` are preserved untouched and remain the authoritative development history.


