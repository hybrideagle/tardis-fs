#!/bin/bash
# ulimit -c unlimited
cd "$MESON_BUILD_ROOT"
ninja
./mkfs.tfs ./a

atom a
