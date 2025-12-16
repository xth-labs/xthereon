#!/usr/bin/env bash
set -e

# Always run from the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

DAEMON="./src/xthereond"
CLI="./src/xthereon-cli"
DATADIR="/home/n3-f0k3-22/.xthereon"
CONF="$DATADIR/xthereon.conf"
RPCUSER="xthuser"
RPCPASS="xthpass123"
RPCPORT="19332"

echo "==============================="
echo " XTH Kill + Reload + Mine Test "
echo "==============================="

echo "[1/7] Killing any running daemon..."
pkill -9 xthereond || true
echo "[1.5/7] Cleaning stale lock files..."

LOCKDIR="$DATADIR/xthereon"

rm -f "$LOCKDIR/.lock"
rm -f "$LOCKDIR/xthereond.pid"

# extra safety (nested leftovers)
find "$LOCKDIR" -name "*.lock" -type f -delete || true

echo "✅ locks cleaned"

sleep 2

if pgrep xthereond >/dev/null; then
  echo "❌ daemon still running"
  exit 1
fi
echo "✅ daemon stopped"

echo "[2/7] Checking ports..."
ss -lntp | grep 19332 && { echo "❌ RPC port still in use"; exit 1; }
ss -lntp | grep 19333 && { echo "❌ P2P port still in use"; exit 1; }
echo "✅ ports free"

echo "[3/7] Starting daemon..."
"$DAEMON" \
  -datadir="$DATADIR" \
  -conf="$CONF" \
  -printtoconsole &
echo "[3.5/7] Waiting for daemon process..."
for i in {1..10}; do
  if pgrep xthereond >/dev/null; then
    echo "✅ daemon running"
    break
  fi
  sleep 1
done

if ! pgrep xthereond >/dev/null; then
  echo "❌ daemon failed to stay running"
  echo "----- last 50 lines of debug.log -----"
  tail -n 50 "$DATADIR/xthereon/debug.log" || true
  exit 1
fi

echo "✅ daemon running"

echo "[4/7] Waiting for RPC..."
for i in {1..10}; do
  if "$CLI" -rpcuser="$RPCUSER" -rpcpassword="$RPCPASS" -rpcport="$RPCPORT" getblockchaininfo >/dev/null 2>&1; then
    echo "✅ RPC ready"
    break
  fi
  sleep 1
done

echo "[5/7] Ensuring wallet exists..."
WALLETS=$("$CLI" -rpcuser="$RPCUSER" -rpcpassword="$RPCPASS" -rpcport="$RPCPORT" listwallets)

if [[ "$WALLETS" == "[]" ]]; then
  echo "→ creating wallet 'main'"
  "$CLI" -rpcuser="$RPCUSER" -rpcpassword="$RPCPASS" -rpcport="$RPCPORT" createwallet "main" >/dev/null
  echo "⚠️ Restart required for wallet load"
  echo "Run script again"
  exit 0
fi

echo "✅ wallet loaded"

echo "[6/7] Enabling mining..."
"$CLI" -rpcuser="$RPCUSER" -rpcpassword="$RPCPASS" -rpcport="$RPCPORT" setgenerate true 1 || true

echo "[7/7] DONE"
echo "-------------------------------"
echo "Mining enabled."
echo "Watch logs with:"
echo "tail -f $DATADIR/xthereon/debug.log"
echo "-------------------------------"
