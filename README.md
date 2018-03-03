# tardis-fs
An implementation of the TARDIS single-file filesystem.

# Why TARDIS?
For those who live under a rock, the name comes from the iconic time-traveling machine from the TV series _Doctor Who_, which is:
1. Awesome
2. But more relevantly, bigger on the inside, which is what a single-file filesystem is supposed to be.

## But what about copyright?
If the BBC sends me a cease-and-desist letter, I will frame it.

# Build instructions
This project uses the meson build system. To use it, you must have meson and ninja-build installed.

1. cd to the project root folder
2. `mkdir ./build && cd build`
3. `meson ..`
4. `ninja`

Recompiling only requires you to rerun `ninja`, if will automatically run `meson` if necessary.

`ninja run` will execute the run script.
`ninja scan-build` runs the Clang static analysis tool, if you have it installed.

# Known issues

1. block_chain_length does not respect half-filled blocks.
2. do_readdir is monstrously complicated.
3. The utils.c functions are not properly tested, will probably bug out later.
