#include "libtfs.h"

void init_backing_storage(char* path){

}

void print_usage(){
    printf("Usage: mkfs.tfs [path/to/backing/file]\n");
}

int main(int argc, char**argv){
    if(argc != 2){
        print_usage();
        return -1;
    }
    init_backing_storage(argv[1]);
    return 0;
}
