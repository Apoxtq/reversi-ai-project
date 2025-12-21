#!/usr/bin/env bash
set -euo pipefail

# clean_dist.sh - keep only N latest packages in dist/
# Usage: ./scripts/clean_dist.sh [KEEP_COUNT]

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DIST="$ROOT/dist"
KEEP=${1:-3}

if [[ ! -d "$DIST" ]]; then
  echo "No dist directory; nothing to clean."
  exit 0
fi

shopt -s nullglob
files=( "$DIST"/*.zip )
if [[ ${#files[@]} -le $KEEP ]]; then
  echo "Nothing to remove (keep=${KEEP})."
  exit 0
fi

# sort by mtime (newest first), remove files after the first $KEEP
mapfile -t sorted < <(ls -1t "$DIST"/*.zip)
for ((i=KEEP;i<${#sorted[@]};i++)); do
  echo "Removing ${sorted[$i]}"
  rm -f -- "${sorted[$i]}"
done

echo "Cleaned dist/ (kept ${KEEP} newest)."


