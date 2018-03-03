#include "include/libtfs.h"

/**
Setup the inital values of files and blocks
*/
void init_internal_data()
{
    for(inode_t inode = 0;inode<NUM_FILES;inode++)
    {
            files[inode].used = false;
    }
    for(blockno_t block = 0;block<NUM_BLOCKS;block++)
    {
            blocks[block].allocated = false;
            blocks[block].next = -1;
    }
    blocks_origin = sizeof(file)*NUM_FILES;
    data_origin = sizeof(file)*NUM_FILES + sizeof(block)*NUM_BLOCKS;

}

void open_backing_storage_file(char* path)
{
    backing_storage_path = path;
    backing_storage = fopen(path,"w+b");
}

void write_backing_storage(char* path)
{
    init_internal_data();
    open_backing_storage_file(path);
    printf("\n[INFO]sizeof(block):%d",sizeof(block));
    printf("\n[INFO]sizeof(file):%d",sizeof(file));
    printf("\nUsing block offset:%d",blocks_origin);
    printf("\nUsing data offset:%d",data_origin);
    printf("\nUsing BLOCKSIZE:%d",BLOCKSIZE);
    printf("\nUsing NUM_FILES:%d",NUM_FILES);
    printf("\nUsing NUM_BLOCKS:%d",NUM_BLOCKS);
    printf("\nInit complete");
    fseek(backing_storage, 0, 0);
    fwrite(files, sizeof(files), NUM_FILES, backing_storage);
    printf("\nFinished writing file list");
    fseek(backing_storage, 0, blocks_origin);
    printf("\nFinished writing block list");
    fwrite(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    printf("\nFinished writing block list");

    //sync();
    fseek(backing_storage, 0, data_origin);
    for(blockno_t record = 0; record < NUM_BLOCKS; record++)
    {
        for(offset_t offset = 0; offset < BLOCKSIZE; offset++)
        {
            fputc((record+offset)%255, backing_storage);
        }
    }
}

void print_usage()
{
    printf("Usage: mkfs.tfs [path/to/backing/file]\n");
}

int main(int argc, char**argv)
{
    if(argc != 2){
        print_usage();
        return -1;
    }
    write_backing_storage(argv[1]);
    printf("\nDone");
    return 0;
}
