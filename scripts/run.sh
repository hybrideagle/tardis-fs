#!/bin/bash
# ulimit -c unlimited
cd "$MESON_BUILD_ROOT"
rm -f blargh
echo "##########################"
echo "Setting up filesystem..."
./mkfs.tfs ./blargh
mkdir x
echo "##########################"
echo "Mounting filesystem..."
./mount.tfs -f ./x 
