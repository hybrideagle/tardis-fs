#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
void display()
{
    START("display");
    int i;
    for (i = 0; i < file_count + 1; i++)
    {
        LOG("\n%s", files[i].path);
    }
}

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
bool valid_path(char* path)
{
    assert(path != NULL);
    assert(path[0] != '\0');

}

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
FILE *get_data_handle(blockno_t block, offset_t offset)
{
    //START("*get_data_handle");
    numassert(offset >= 0 && offset < BLOCKSIZE, offset);
    numassert(block >= 0 && block < NUM_BLOCKS, block);
    assert(backing_storage_path != NULL);
    FILE *handle = fopen(backing_storage_path, "rw+b");
    fseek(handle, (block * BLOCKSIZE) + offset, blocks_origin);

    //LOG("%d , %d",(block * BLOCKSIZE) + offset,blocks_origin);
    //LOG("get_data_handle : handle : %d",handle);
    //LOG("%c",getc(handle));

//    LOG("%d",(block * BLOCKSIZE) + offset);
    return handle;
    END("*get_data_handle");
}

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
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

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
void dump_data()
{
    assertd(backing_storage_path != NULL);
    numassert(file_count >= 0, file_count);

    LOG("\n\n>>>>>>>>>>>>>>>>>>");
    LOG("\nFILE METADATA:");
    LOG("\tinode\tused\tfirst\tis_dir\tpath");
    for(inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        LOG("\t%d\t%d\t%d\t%d\t%s", inode, files[inode].used,files[inode].start_block,files[inode].is_dir,files[inode].path);
    }

    LOG("\n\n>>>");

    LOG("\nBLOCK METADATA:");
    LOG("\tblockno\tallocated\tnext\tdata\tlen");
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        FILE* h = get_data_handle(block, 0);
        char data[BLOCKSIZE];
        read_from_block(block, 0, data, BLOCKSIZE);
        LOG1("\t%d\t%d\t%d\t%s\t%d", block, blocks[block].allocated, blocks[block].next,data,strlen(data));
    }
    LOG("\n>>>>>>>>>>>>>>>>>>\n\n");
}

/**
* @brief <brief>
* @param [in] <name> <parameter_description>
* @return <return_description>
* @details <details>
*/
void init_logging()
{
    //log_file = fopen("log", 'a');
}

void nullify_path_array(char* path)
{
    for(int i = 0;i < PATH_LENGTH; i++)
        path[i] = '\0';
}
