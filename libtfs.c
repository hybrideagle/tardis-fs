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
    START("*get_data_handle");
    assert(offset < BLOCKSIZE);

    FILE *handle = fopen(backing_storage_path, "rw+b");
    fseek(handle, (block * BLOCKSIZE) + offset, blocks_origin);
    return handle;

    END("*get_data_handle");
}

/**
   read `bytes` bytes from the file into `buffer`.
   `block` points to the beginning of the file
   returns 1 on failure
   No bound checking occurs, segfaults are your problem
 */
int read_from_block(blockno_t block, offset_t offset, char *buffer, int bytes)
{
    START("read_from_block");
    //seek to the correct block
    while (offset > BLOCKSIZE)
    {
        offset -= BLOCKSIZE;
        block = get_next_block(block);
        if (-1 == block)
        {
            printf("\nFile overread");
            return -1;

            END("read_from_block");
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
    START("read_from_path");
    return read_from_block(get_first_block_from_path(path), offset, buffer, bytes);

    END("read_from_path");
}

//auto-skips if offset and/or (offset+bytes) is greater than block size.
int write_to_block(blockno_t block, offset_t offset, char *buffer, int bytes)
{
    START("write_to_block");
    //seek to the correct block
    while (offset > BLOCKSIZE)
    {
        offset -= BLOCKSIZE;
        block = get_next_block(block);
        if (-1 == block)
        {
            printf("\nFile overread");
            return -1;

            END("write_to_block");
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
    START("write_to_path");
    return write_to_block(get_first_block_from_path(path), offset, buffer, bytes);

    END("write_to_path");
}

blockno_t get_first_block_from_path(char *path)
{
    START("get_first_block_from_path");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (0 == strcmp(files[inode].path, path))
        {
            return get_first_block_from_inode(inode);

            END("get_first_block_from_path");
        }
    }
    return -1;
}

blockno_t get_first_block_from_inode(inode_t inode)
{
    START("get_first_block_from_inode");
    return files[inode].start_block;

    END("get_first_block_from_inode");
}

blockno_t get_next_block(blockno_t blockno)
{
    START("get_next_block");
    return blocks[blockno].next;

    END("get_next_block");
}

blockno_t get_or_create_next_block(blockno_t blockno)
{
    START("get_or_create_next_block");
    // If next isn't allocated, allocate it
    if (blocks[blockno].next == -1)
    {
        //TODO finish this
        for (blockno_t next = 0; next < NUM_BLOCKS; next++)
        {
            if (blocks[next].allocated == false)
            {
                blocks[blockno].next = next;
                assert(blocks[next].next == -1);
                blocks[next].allocated = true;
                END("get_or_create_next_block");
            }
        }
    }

    return blocks[blockno].next;
}

blockno_t get_first_free_block()
{
    START("get_first_free_block");
    for (blockno_t i = 0; i < NUM_BLOCKS; i++)
    {
        if (!blocks[i].allocated)
        {
            return i;

            END("get_first_free_block");
        }
    }
    return -1;
}

bool delete_block_chain(blockno_t start_block)
{
    START("delete_block_chain");
    if (start_block == -1)
    {
        return false;

        END("delete_block_chain");
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
    START("create_file");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = true;
            files[inode].is_dir = true;
            //files[inode].path = path
            strcpy(files[inode].path, path);
            files[inode].start_block = get_first_free_block();
            return inode;

            END("create_file");
        }
    }
    return -1;
}

int delete_file(char *path)
{
    START("delete_file");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = false;
            free(files[inode].path);
            delete_block_chain(files[inode].start_block);
            return true;

            END("delete_file");
        }
    }
    return false;
}

inode_t create_dir(char *path)
{
    START("create_dir");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = true;
            files[inode].is_dir = true;
            //files[inode].path = path
            strcpy(files[inode].path, path);
            files[inode].start_block = get_first_free_block();
            return inode;

            END("create_dir");
        }
    }
    return -1;
}

int delete_dir(char *path)
{
    START("delete_dir");
    // TODO check for subfiles and shit
    return delete_file(path);

    END("delete_dir");
}

void init_tfs(char *path)
{
    START("init_tfs");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        files[inode].used = false;
        END("init_tfs");
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

//TODO make this function respect half-filled blocks
int block_chain_length(blockno_t start_block)
{
    START("block_chain_length");
    int size = 0;
    do
    {
        size += BLOCKSIZE;
        start_block = blocks[start_block].next;
        END("block_chain_length");
    } while (start_block != -1);
    return size;
}
