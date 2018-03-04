#include "include/libtfs.h"

/**
Setup the inital values of files and blocks
*/

void open_backing_storage_file(char* path)
{
    backing_storage_path = path;
    backing_storage = fopen(path,"w+b");
}

void write_backing_storage(char* path)
{
    init_tfs(NULL, false);
    blocks[3].allocated = true;
    open_backing_storage_file(path);
    sync_metadata();

    //sync_metadata();
    char filler[BLOCKSIZE];
    for(int i = 0;i<BLOCKSIZE;i++)
        filler[i] = 5;

    for(blockno_t record = 0; record < NUM_BLOCKS; record++)
    {
        pwrite(fileno(backing_storage), filler, BLOCKSIZE, data_origin + record*BLOCKSIZE);
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
    dump_data();
    printf("\nDone");
    return 0;
}
