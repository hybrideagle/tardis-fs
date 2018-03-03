project('tfs','c')

# Common compiler/linker args
cpp_args = ['-D_FILE_OFFSET_BITS=64', '-I/usr/include/fuse', '-g']
link_args = ['-lfuse','-pthread']

# Import file objects from subdirectories
subdir('libfuse')
subdir('mount')
subdir('mkfs')

# Build the shared object for libfuse
libfuse = shared_library('libfuse', libfuse_sources)
libfuse_includes = include_directories('include')

# set up the mkfs.tfs executable
mkfs_name = 'mkfs.tfs'
executable(mkfs_name,mkfs_sources, cpp_args: cpp_args, link_args: link_args, include_directories: libfuse_includes, link_with: libfuse)

# Set up building the mount.tfs executable
mount_name = 'mount.tfs'
executable(mount_name, mount_sources, cpp_args: cpp_args, link_args: link_args, include_directories: libfuse_includes, link_with: libfuse)
