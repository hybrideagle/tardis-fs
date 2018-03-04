#!/bin/bash
# ulimit -c unlimited
cd "$MESON_BUILD_ROOT"
fusermount -u x
rm -f blargh
echo "##########################"
echo "Setting up filesystem..."
./mkfs.tfs ./blargh
mkdir x
clear
echo "##########################"
echo "Mounting filesystem..."
./mount.tfs -f ./x
