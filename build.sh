#!/usr/bin/env bash
# Configure and build the firmware. Usage: ./build.sh [debug|release]
set -euo pipefail

PRESET="${1:-debug}"

cmake --preset "$PRESET"
cmake --build --preset "$PRESET"
