//%cflags:  -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
#include <string.h>
#include <unistd.h>
#include "libtfs.h"
#include <assert.h>

//TODO add file size calculation
//TODO add directories

//TODO rename this
FILE *get_data_handle(blockno_t block, offset_t offset)
{
    assert(offset < BLOCKSIZE);

    FILE *handle = fopen(backing_storage_path, "rw+b");
    fseek(handle, (block * BLOCKSIZE) + offset, blocks_origin);
    return handle;
}

/**
   read `bytes` bytes from the file into `buffer`.
   `block` points to the beginning of the file
   returns 1 on failure
   No bound checking occurs, segfaults are your problem
 */
int read_from_block(blockno_t block, offset_t offset, char *buffer, int bytes)
{
    //seek to the correct block
    while (offset > BLOCKSIZE)
    {
        offset -= BLOCKSIZE;
        block = get_next_block(block);
        if (-1 == block)
        {
            printf("\nFile overread");
            return -1;
        }
    }
    // start reading data
    FILE *handle;
    while (bytes > 0)
    {
        handle = get_data_handle(block, offset);
        offset = 0;
        for (int i = 0; i < BLOCKSIZE && bytes > 0; i++)
        {
            *(buffer++) = fgetc(handle);
            bytes--;
        }
        fclose(handle);
        block = get_next_block(block);
    }
    return 0;
}

int read_from_path(char *path, offset_t offset, char *buffer, int bytes)
{
    return read_from_block(get_first_block_from_path(path), offset, buffer, bytes);
}

//auto-skips if offset and/or (offset+bytes) is greater than block size.
int write_to_block(blockno_t block, offset_t offset, char *buffer, int bytes)
{
    //seek to the correct block
    while (offset > BLOCKSIZE)
    {
        offset -= BLOCKSIZE;
        block = get_next_block(block);
        if (-1 == block)
        {
            printf("\nFile overread");
            return -1;
        }
    }
    // start writing data
    FILE *handle;
    while (bytes > 0)
    {
        handle = get_data_handle(block, offset);
        offset = 0;
        for (int i = 0; i < BLOCKSIZE && bytes > 0; i++)
        {
            fputc(*buffer++, handle);
            bytes--;
        }
        fclose(handle);
        block = get_or_create_next_block(block);
    }
    return 0;
}

int write_to_path(char *path, offset_t offset, char *buffer, int bytes)
{
    return write_to_block(get_first_block_from_path(path), offset, buffer, bytes);
}

blockno_t get_first_block_from_path(char *path)
{
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (0 == strcmp(files[inode].path, path))
        {
            return get_first_block_from_inode(inode);
        }
    }
    return -1;
}

blockno_t get_first_block_from_inode(inode_t inode)
{
    return files[inode].start_block;
}

blockno_t get_next_block(blockno_t blockno)
{
    return blocks[blockno].next;
}

blockno_t get_or_create_next_block(blockno_t blockno)
{
    if(blocks[blockno].next == -1)
    {
        //TODO finish this
        for()
    }

    return blocks[blockno].next;
}

blockno_t get_first_free_block()
{
    for (blockno_t i = 0; i < NUM_BLOCKS; i++)
    {
        if (!blocks[i].allocated)
        {
            return i;
        }
    }
    return -1;
}

bool delete_block_chain(blockno_t start_block)
{
    if (start_block == -1)
    {
        return false;
    }
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
    fseek(backing_storage, 0, 0);
    fread(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_origin);
    fread(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    printf("Done");
}

void sync()
{
    files_origin = 0;
    blocks_origin = sizeof(file) * NUM_FILES;
    data_origin = sizeof(file) * NUM_FILES + sizeof(blocks) * NUM_BLOCKS;
    fseek(backing_storage, 0, 0);
    fwrite(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_origin);
    fwrite(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    fsync(fileno(backing_storage));
}
