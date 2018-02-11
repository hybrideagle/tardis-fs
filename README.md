# tardis-fs
An implementation of the TARDIS single-file filesystem.

# Why TARDIS?
For those who live under a rock, the name comes from the iconic time-traveling machine from the TV series _Doctor Who_, which is:
1. Awesome
2. But more relevantly, bigger on the inside, which is what a single file filesystem is supposed to be.

## But what about copyright?
If BBC sends me a cease-and-desist letter, I will frame it.

# Build instructions
This project uses the meson build system. To use it, you must have meson and ninja-build installed.

1. cd to the project root folder
2. `mkdir ./build && cd build`
3. `meson ..`
4. `ninja`

Recompiling only requires you to rerun `ninja`, if will automatically run `meson` if necessary.
If you really hate anything new or cool, there is also a (outdated) `make` file.
Good look getting it working.

# Known issues

1. The `libtfs.h` header is getting included multiple times, despite the header guard.
Could cause problems later.
