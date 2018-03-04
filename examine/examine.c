#include "include/libtfs.h"

/**
Setup the inital values of files and blocks
*/

void open_backing_storage_file(char* path)
{
    backing_storage_path = path;
    backing_storage = fopen(path,"w+b");
}

void dump_all_data(char* path)
{
    open_backing_storage_file(path);
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
    if(argc != 2){
        print_usage();
        return -1;
    }
    dump_all_data(argv[1]);
    return 0;
}
