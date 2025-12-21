# Changelog

All notable changes to this project are documented here.

## [1.1.0] - 2025-12-21
- Added conditional packaging flag `-DENABLE_PACKAGING` and CPack integration.
- Added cross-platform packaging scripts (`scripts/build_release.*`) and cleanup scripts.
- Consolidated documentation into `docs/01_Overview.md`, `docs/02_Build_and_Dev.md`, `docs/03_Packaging_and_Release.md`.
- Added CI workflow for artifacts-only packaging on tag/dispatch.
- Performance and algorithm improvements:
  - PVS + killer + aspiration parameter tuning; `Config::preset_fixed_found()` and `preset_optimized()` added.
  - Memory reuse in `MinimaxEngine` and in-place board move application to reduce allocations.
- Tests & benchmarks:
  - `bench_week6` and `test_week6` extended; added PVS diagnostics.
  - Added parameter search tools (`param_search`, `param_opt`) and `stepwise_rollout`.

## [1.0.0] - previous
- Initial baseline (project bootstrapped and Week1–Week11 features implemented).


