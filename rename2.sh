#!/usr/bin/env bash
set -euo pipefail

# Words → Replace
declare -A REPL
REPL["XThereon"]="XThereon"
REPL["XTHEREON"]="XTHEREON"
REPL["xthereon"]="xthereon"
REPL["XThereon"]="XThereon"
REPL["XTHEREON"]="XTHEREON"
REPL["xthereon"]="xthereon"

# Directories to skip
SKIP_DIRS=(
  ".git"
  "depends"
  "src/secp256k1"
  "src/leveldb"
  "src/univalue"
  "src/crypto/ctaes"
)

# Extensions allowed
EXTS=(
  h hpp c cc cpp cxx m mm
  py sh bash json yml yaml toml txt md rst
  conf cfg ini pro pri cmake in ac am
  qrc ui ts desktop
)

echo ">>> Starting safe rename..."

# Build skip args
SKIP_ARGS=()
for d in "${SKIP_DIRS[@]}"; do
  SKIP_ARGS+=(-path "./$d" -prune -o)
done

# Build extension args
EXT_ARGS=()
for e in "${EXTS[@]}"; do
  EXT_ARGS+=(-name "*.$e" -o)
done
EXT_ARGS=("${EXT_ARGS[@]::${#EXT_ARGS[@]}-1}") # remove last -o

# -------------------------------
# In-file replacements
# -------------------------------
echo ">>> Replacing inside files..."

find . \
  "${SKIP_ARGS[@]}" \
  \( "${EXT_ARGS[@]}" \) -print0 |
while IFS= read -r -d '' f; do
  for from in "${!REPL[@]}"; do
    to="${REPL[$from]}"
    sed -i "s/$from/$to/g" "$f"
  done
done

# -------------------------------
# Rename filenames & dirs
# -------------------------------
echo ">>> Renaming filenames..."

find . -depth -print0 | while IFS= read -r -d '' p; do

  # Skip special directories
  for s in "${SKIP_DIRS[@]}"; do
    [[ "$p" == "./$s"* ]] && continue 2
  done

  new="$p"
  for from in "${!REPL[@]}"; do
    to="${REPL[$from]}"
    new="${new//$from/$to}"
  done

  if [[ "$new" != "$p" ]]; then
    echo "git mv \"$p\" \"$new\""
    git mv "$p" "$new"
  fi
done

echo ">>> DONE. Review with: git status && git diff"
