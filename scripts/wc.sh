#!/bin/bash
cd "$MESON_SOURCE_ROOT"

gcc -fpreprocessed -dD -E  */*.c */*.h  | wc
