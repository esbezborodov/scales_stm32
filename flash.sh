#!/usr/bin/env bash
# Build and flash the board through an ST-LINK (SWD).
# Usage: ./flash.sh [debug|release|bluepill-debug|bluepill-release]
set -euo pipefail

cd "$(dirname "$0")"
PRESET="${1:-debug}"
ELF="build/${PRESET}/scale_firmware.elf"

./build.sh "$PRESET"

openocd -f firmware/debug/openocd.cfg \
    -c "program ${ELF} verify reset exit"
