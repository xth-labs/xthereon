#!/usr/bin/env bash
set -euo pipefail

# === CONFIG ===
# Adjust these if you want different capitalization
OLD_PASCAL="XThereon"
NEW_PASCAL="XThereon"

OLD_UPPER="XTHEREON"
NEW_UPPER="XTHEREON"

OLD_LOWER="xthereon"
NEW_LOWER="xthereon"

# Extensions we consider "safe" to touch (text / code files)
EXTENSIONS=(
  "*.h" "*.hpp" "*.c" "*.cc" "*.cpp" "*.cxx"
  "*.moc" "*.mm"
  "*.py" "*.sh" "*.bash"
  "*.pro" "*.pri"
  "*.qrc" "*.ui" "*.ts"
  "*.json" "*.yml" "*.yaml" "*.toml"
  "*.in" "*.ac" "*.am" "*.cmake" "CMakeLists.txt"
  "*.md" "*.rst" "*.txt"
  "*.conf" "*.cfg" "*.ini"
  "*.desktop"
)

# Paths we definitely do NOT want to touch (git & some third-party libs, adjust as needed)
EXCLUDES=(
  "./.git/*"
  "./depends/*"
  "./src/secp256k1/*"
  "./src/leveldb/*"
  "./src/univalue/*"
  "./src/crypto/ctaes/*"
)

echo ">>> Renaming inside files: ${OLD_PASCAL}/${OLD_UPPER}/${OLD_LOWER} -> ${NEW_PASCAL}/${NEW_UPPER}/${NEW_LOWER}"
echo ">>> Make sure your git working tree is clean before running this!"
echo

# Build find predicates for extensions
ext_predicate=""
for ext in "${EXTENSIONS[@]}"; do
  if [[ -z "$ext_predicate" ]]; then
    ext_predicate="-name \"$ext\""
  else
    ext_predicate="$ext_predicate -o -name \"$ext\""
  fi
done

# Build predicates for excludes
exclude_predicate=""
for ex in "${EXCLUDES[@]}"; do
  exclude_predicate="$exclude_predicate ! -path \"$ex\""
done

# === 1) In-file replacements ===
# We use 'eval' because we're building the find command as a string.
cmd="find . -type f \( $ext_predicate \) $exclude_predicate -print0"
echo "Running content replacement with:"
echo "  $cmd"
echo

eval "$cmd" | while IFS= read -r -d '' file; do
  # Use perl for in-place regex-safe replacement
  perl -pi -e "s/\b${OLD_PASCAL}\b/${NEW_PASCAL}/g" "$file"
  perl -pi -e "s/\b${OLD_UPPER}\b/${NEW_UPPER}/g" "$file"
  perl -pi -e "s/\b${OLD_LOWER}\b/${NEW_LOWER}/g" "$file"
done

echo ">>> In-file replacements done."
echo

# === 2) File and directory name renames ===
echo ">>> Renaming files and directories containing XThereon variants using git mv"
echo "    (this keeps git history clean)."
echo

rename_pattern() {
  local pattern="$1"
  local from="$2"
  local to="$3"

  echo "Handling names matching pattern: $pattern   ($from -> $to)"

  # -depth ensures we rename files before their parent directories
  eval "find . -depth $exclude_predicate -name \"$pattern\" -print0" | \
  while IFS= read -r -d '' path; do
    new_path="${path//$from/$to}"
    if [[ "$path" != "$new_path" ]]; then
      echo "  git mv \"$path\" \"$new_path\""
      git mv "$path" "$new_path"
    fi
  done
}

# PascalCase
rename_pattern "*${OLD_PASCAL}*" "${OLD_PASCAL}" "${NEW_PASCAL}"
# UPPER
rename_pattern "*${OLD_UPPER}*" "${OLD_UPPER}" "${NEW_UPPER}"
# lower
rename_pattern "*${OLD_LOWER}*" "${OLD_LOWER}" "${NEW_LOWER}"

echo
echo ">>> All done."
echo ">>> Review changes with:  git status && git diff"
echo ">>> If happy, commit them."
