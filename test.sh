#!/usr/bin/env bash
# Build and run the host unit tests (needs a native C compiler).
set -euo pipefail

cd "$(dirname "$0")"
cmake -S firmware/tests -B build/tests -G Ninja
cmake --build build/tests
ctest --test-dir build/tests --output-on-failure
