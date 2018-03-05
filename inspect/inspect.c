#include "include/libtfs.h"

/**
Setup the inital values of files and blocks
*/

void dump_all_data(char* path)
{
    printf("\nsizeof(block):%d",sizeof(block));
    printf("\nsizeof(file):%d",sizeof(file));
    printf("\nUsing block offset:%d",blocks_origin);
    printf("\nUsing data offset:%d",data_origin);
    printf("\nUsing BLOCKSIZE:%d",BLOCKSIZE);
    printf("\nUsing NUM_FILES:%d",NUM_FILES);
    printf("\nUsing NUM_BLOCKS:%d",NUM_BLOCKS);
    pread(fileno(backing_storage), files, sizeof(files), files_origin);
    pread(fileno(backing_storage),blocks, sizeof(blocks), blocks_origin);
    dump_data();
}

void print_usage()
{
    printf("Usage: mkfs.tfs [path/to/backing/file]\n");
}

int main(int argc, char**argv)
{
    init_tfs(argv[1], true);
    LOG("done with init");
    if(argc != 2){
        print_usage();
        return -1;
    }
    dump_all_data(argv[1]);
    return 0;
}
