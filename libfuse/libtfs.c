//%cflags:  -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

//TODO add file size calculation
//TODO add directories

//TODO rename this

//TODO make this take a path from command line

/**
 * @brief Sets up the internal data
 * @details This function should be called right at the beginning of main(). Must be called before any libtfs functions
 */
void init_tfs(char* path, bool read_from_backing_storage)
{
    files_origin = 0;
    blocks_origin = files_origin + sizeof(file) * NUM_FILES;
    data_origin = blocks_origin + sizeof(block) * NUM_BLOCKS;
    LOG("setting data origin:%d, %d",data_origin);
    backing_storage_path = (path==NULL)?"blargh":path;

    START("init_tfs");

    if(!read_from_backing_storage)
    {
        LOG("Using default data");
        for (inode_t inode = 0; inode < NUM_FILES; inode++)
        {
            files[inode].used = false;
            for(int i = 0; i < PATH_LENGTH; i++)
            //        files[inode].path[i] = '\0';
            files[inode].path[i] = 7;

        }
        LOG1("initialized files");
        for (blockno_t block = 0; block < NUM_BLOCKS; block++)
        {
            blocks[block].allocated = false;
            blocks[block].next = -1;
        }
        LOG1("initialized blocks");
    }

    else
    {
        LOG("Reading from backing storage");
        backing_storage = fopen(backing_storage_path, "r+");
        pread(fileno(backing_storage), files, sizeof(files), files_origin);
        pread(fileno(backing_storage), blocks, sizeof(blocks), blocks_origin);
//        sync_metadata();
    //    data_dump();
    }
    LOG1("data dump:");
    LOG1("NUM_FILES:%d", NUM_FILES);
    LOG1("NUM_BLOCKS:%d", NUM_BLOCKS);
    LOG1("PATH_LENGTH:%d", PATH_LENGTH);
    LOG1("files_origin:%d", files_origin);
    LOG1("blocks_origin:%d", blocks_origin);
    LOG1("data_origin:%d", data_origin);
    LOG1("sizeof(block):%d", sizeof(struct block));
    LOG1("sizeof(file):%d", sizeof(struct file));
    END("init_tfs");
//    dump_data();
//    sanity_check();
}

/**
 * @brief Sync all the data to backing store
 * @details Writes all the metadata(not thread-safe), and then calls fsync to update the data.
 */
void sync_metadata()
{
    START("sync");
    pwrite(fileno(backing_storage), files, sizeof(files), files_origin);
    pwrite(fileno(backing_storage), blocks, sizeof(blocks), blocks_origin);
    //fsync(fileno(backing_storage));
    END("sync");
}
