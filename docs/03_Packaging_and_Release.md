# Packaging & Release

This consolidates packaging policy, checklist, and changelog.

Packaging policy
- Packaging is disabled by default. Enable with `-DENABLE_PACKAGING=ON` or use `scripts/build_release.*`.
- Packages are written to `dist/` and `dist/` is ignored by git.
- CI workflow produces artifacts on tag or manual dispatch; by default artifacts are stored by CI, not pushed to external services.

Release checklist (summary)
- Verify version in `CMakeLists.txt`.
- Build & test on MSVC and MinGW following `docs/02_Build_and_Dev.md`.
- Run benchmarks (`bench_week6`) and record results.
- Update `docs/CHANGELOG.md`.
- Run `scripts/build_release.*` to produce ZIP (optional), verify contents, then `scripts/clean_dist.* 0` to remove test packages.
- Tag and push annotated git tag `vX.Y.Z`.

CHANGELOG (Unreleased)
- Added packaging scripts (Windows + POSIX) and CI artifacts-only packaging.
- Added conditional packaging flag `-DENABLE_PACKAGING` to CMake.
- Consolidated docs into `docs/` and added release checklist.
- Performance tuning:
  - PVS + killer + aspiration parameter tuning; added `Config::preset_fixed_found()` and `preset_optimized()`.
  - Memory reuse in `MinimaxEngine` and in-place board moves to reduce allocations.
- Tests/benchmarks:
  - `bench_week6`, `test_week6` fixes and PVS diagnostics.
  - Added `stepwise_rollout` and parameter search tools.


