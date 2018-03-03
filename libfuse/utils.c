#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

void display()
{
    START("display");
    int i;
    for (i = 0; i < file_count + 1; i++)
    {
        LOG("\n%s", files[i].path);
    }
}

bool valid_path(char* path)
{
    assert(path != NULL);
    assert(path[0] != '\0');

}

FILE *get_data_handle(blockno_t block, offset_t offset)
{
    START("*get_data_handle");
    numassert(offset >= 0 && offset < BLOCKSIZE, offset);
    numassert(block >= 0 && block < NUM_BLOCKS, block);
    assert(valid_path(backing_storage_path));
    FILE *handle = fopen(backing_storage_path, "rw+b");
    fseek(handle, (block * BLOCKSIZE) + offset, blocks_origin);
    return handle;

    END("*get_data_handle");
}

void sanity_check()
{
    assertd(backing_storage_path != NULL);
    numassert(file_count >= 0, file_count);
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        if (blocks[block].allocated == false)
        {
            numassert(blocks[block].next == -1, block);
        }
    }
    for(inode_t inode = 0; inode < file_count; inode++)
    {
        if(files[inode].used == true)
        {
            assertd(files[inode].path != NULL);
            numassert(files[inode].start_block >= 0 && files[inode].start_block <= NUM_BLOCKS, files[inode].start_block);
        }
    }
}

void dump_data()
{
    assertd(backing_storage_path != NULL);
    numassert(file_count >= 0, file_count);

    printf("\n\n>>>>>>>>>>>>>>>>>>");
    printf("\nBLOCK METADATA:");
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        printf("\nBlock %d:\n", block);
        printf("\n\tallocated:%d", blocks[block].allocated);
        printf("\n\tnext:%d", blocks[block].next);

    }
    printf("\n\n>>>");
    printf("\nFILE METADATA:");
    for(inode_t inode = 0; inode < file_count; inode++)
    {
        printf("\nFile %d:\n", inode);
        printf("\n\tused:%d", files[inode].used);
        printf("\n\tis_dir:%d", files[inode].is_dir);
        printf("\n\tpath:%s", files[inode].path);
    }
    printf("\n>>>>>>>>>>>>>>>>>>\n\n");
}
