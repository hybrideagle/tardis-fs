#!/bin/bash
# ulimit -c unlimited
cd "$MESON_BUILD_ROOT"
rm -f blargh
echo "##########################"
echo "Setting up filesystem..."
./mkfs.tfs ./blargh
mkdir /tmp/a
echo "##########################"
echo "Mounting filesystem..."
./mount.tfs ./blargh /tmp/a -f -v
