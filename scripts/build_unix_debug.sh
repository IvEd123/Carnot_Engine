#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_TYPE=Debug
OUT_DIR="$ROOT_DIR/build"

conan install "$ROOT_DIR" -of $OUT_DIR --build=missing -s build_type="$BUILD_TYPE" 
  -c tools.cmake.cmaketoolchain:extra_variables='{"CMAKE_POLICY_VERSION_MINIMUM":"3.5"}'

cmake -S "$ROOT_DIR" -B "$OUT_DIR/build/$BUILD_TYPE" \
 # -DCMAKE_TOOLCHAIN_FILE="$OUT_DIR/build/$BUILD_TYPE/generators/conan_toolchain.cmake" \
 --preset conan-debug \ 
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5

cmake --build "$OUT_DIR/build/$BUILD_TYPE"
