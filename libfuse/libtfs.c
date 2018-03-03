//%cflags:  -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

//TODO add file size calculation
//TODO add directories

//TODO rename this

//TODO make this take a path from command line
void init_tfs()
{
    backing_storage_path = (char*)malloc(100);
    sprintf(backing_storage_path, "blargh");

    START("init_tfs");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        files[inode].used = false;
    }
    LOG1("initialized files");
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        blocks[block].allocated = false;
        blocks[block].next = -1;
    }
    LOG1("initialized blocks");

    backing_storage = fopen(backing_storage_path, "w+");
    int backing_storage_fd = fileno(backing_storage);

    pread(backing_storage_fd, files, sizeof(files) * NUM_FILES, files_origin);
    pread(backing_storage_fd, blocks, sizeof(block) * NUM_BLOCKS, blocks_origin);

    END("init_tfs");
    sanity_check();
}

void sync()
{
    START("sync");
    files_origin = 0;
    blocks_origin = sizeof(file) * NUM_FILES;
    data_origin = sizeof(file) * NUM_FILES + sizeof(blocks) * NUM_BLOCKS;
    fseek(backing_storage, 0, 0);
    fwrite(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_origin);
    fwrite(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    fsync(fileno(backing_storage));
    END("sync");
}
