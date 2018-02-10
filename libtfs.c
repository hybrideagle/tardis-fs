//%cflags:  -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
#include <string.h>
#include <unistd.h>
#include "libtfs.h"

int read_from_block(blockno_t n, char *buffer, int bytes, int offset)
{
}

int read_from_path(char *path, char *buffer, int bytes, int offset)
{
    return(read_from_block(get_first_block_from_path(path), buffer, bytes, offset));
}

//auto-skips if offset and/or (offset+bytes) is greater than block size.
int write_to_block(blockno_t n, char *buffer, int bytes, int offset)
{
}

int write_to_path(char *path, char *buffer, int bytes, int offset)
{
    return write_to_block(get_first_block_from_path(path), buffer, bytes, offset);
}

blockno_t get_first_block_from_path(char *path)
{
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (strcmp(files[inode].path, path) == 0)
        {
            return get_first_block_from_inode(inode);
        }
    }
    return -1;
}

blockno_t get_first_block_from_inode(inode_t inode)
{
    return(files[inode].start_block);
}

blockno_t get_next_block(blockno_t blockno)
{
    return(blocks[blockno].next);
}

blockno_t get_first_free_block()
{
    for (blockno_t i = 0; i < NUM_BLOCKS; i++)
    {
        if (!blocks[i].allocated)
        {
            return(i);
        }
    }
    return -1;
}

bool delete_block_chain(blockno_t start_block)
{
    if(start_block == -1)
        return false;
    blockno_t curr_block = start_block;

    do
    {
        blocks[curr_block].next = -1;
        blocks[curr_block].allocated = false;
        curr_block = blocks[curr_block].next;
    } while (curr_block != -1);

    return true;
}

inode_t create_file(char *path)
{
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = true;
            files[inode].path = path;
            files[inode].start_block = get_first_free_block();
            return inode;
        }
    }
    return -1;
}

int delete_file(char *path)
{
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = false;
            free(files[inode].path);
            delete_block_chain(files[inode].start_block);
            return true;
        }
    }
    return false;
}

void init_tfs(char *path)
{
    printf("\nInitializing the file system...");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        files[inode].used = false;
    }
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        blocks[block].allocated = false;
        blocks[block].next = -1;
    }
    backing_storage_path = strdup(path);
    backing_storage = fopen(path, "r+b");
    fseek(backing_storage, 0, 0);
    fread(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_offset);
    fread(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    printf("Done");
}

void sync()
{
    files_offset = 0;
    blocks_offset = sizeof(file) * NUM_FILES;
    data_offset = sizeof(file) * NUM_FILES + sizeof(blocks) * NUM_BLOCKS;
    fseek(backing_storage, 0, 0);
    fwrite(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_offset);
    fwrite(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    fsync(fileno(backing_storage));
}
