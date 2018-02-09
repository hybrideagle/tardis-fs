#include "libtfs.h"

int read_from_block(blockno_t n, char* buffer, int bytes, int offset){

}

int read_from_path(char* path, char* buffer, int bytes, int offset){
        return read_from_block(get_first_block_from_path(path), buffer, bytes, offset);
}

//auto-skips if offset and/or (offset+bytes) is greater than block size.
int write_to_block(blockno_t n, char* buffer, int bytes, int offset){
}

int write_to_path(char* path, char* buffer, int bytes, int offset){
        return write_to_block(get_first_block_from_path(path), buffer, bytes, offset);
}

blockno_t get_first_block_from_path(char* path){

}

blockno_t get_first_block_from_inode(inode_t inode){
        return files[inode].start_block;
}

blockno_t get_next_block(blockno_t blockno){
        return blocks[blockno].next;
}

inode_t create_file(char* path){
}

int delete_file(char* path){
}

void init_tfs(char* path){
}
