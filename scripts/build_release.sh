#!/usr/bin/env bash
set -euo pipefail

# build_release.sh - build and (optionally) package release artifacts into dist/
# Usage:
#   ./scripts/build_release.sh        # build and package (default)
#   ./scripts/build_release.sh --no-packaging

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build_release"
DIST_DIR="$ROOT/dist"
ENABLE_PACKAGING=1
if [[ "${1:-}" == "--no-packaging" ]]; then
  ENABLE_PACKAGING=0
fi

mkdir -p "$DIST_DIR" "$BUILD_DIR"
cd "$BUILD_DIR"

if [[ $ENABLE_PACKAGING -eq 1 ]]; then
  cmake -G Ninja -DENABLE_PACKAGING=ON -DCMAKE_BUILD_TYPE=Release ..
else
  cmake -G Ninja -DENABLE_PACKAGING=OFF -DCMAKE_BUILD_TYPE=Release ..
fi

ninja -j"$(nproc)"

if [[ $ENABLE_PACKAGING -eq 1 ]]; then
  echo "Running CPack..."
  cpack -C Release
  # rename latest zip to include timestamp and arch
  latest="$(ls -1t "$DIST_DIR"/*.zip 2>/dev/null | head -n1 || true)"
  if [[ -n "$latest" ]]; then
    ts="$(date +%Y%m%d-%H%M%S)"
    arch="$(uname -m)"
    newname="$DIST_DIR/ReversiAI-${ts}-${arch}.zip"
    mv "$latest" "$newname"
    echo "Packaged to: $newname"
  else
    echo "No package produced by CPack."
  fi
fi

echo "build_release completed."


