project('tfs','c')
executable('mount.tfs',['tfs.c','libtfs.h', 'libtfs.c', 'file_operations.c','block_operations.c', 'readwrite.c'], cpp_args: '-D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -g ', link_args: ['-lfuse','-pthread'])
executable('mkfs.tfs',['mkfs.c','libtfs.h'], cpp_args: '-D_FILE_OFFSET_BITS=64 -I/usr/include/fuse', link_args: ['-lfuse','-pthread'])
