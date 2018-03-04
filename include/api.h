#ifndef API_H
#define API_H
#include "defs.h"

extern inode_t create_file(char* path);
extern int delete_file(char* path);


extern inode_t create_dir(char* path);
extern int delete_dir(char* path);


void display();
void sanity_check();
void dump_data();

void nullify_path_array(char* path);

// for each block(as the index of the array), stores the next block's number
extern blockno_t get_first_block_from_path(char* path);
extern blockno_t get_first_block_from_inode(inode_t inode);
extern blockno_t get_next_block();
extern blockno_t get_or_create_next_block(blockno_t blockno);
extern blockno_t get_first_free_block();
extern bool delete_block_chain(blockno_t);
// void delete_block(blockno_t); //Don't think we'll need this

// basic read and write function with 2 ways of accessing the file.
// Returns 0 on success, -1 on failure
// It's your problem if it segfaults because bytes > len(buffer)
extern int read_from_block(blockno_t n, offset_t offset, char *buffer, int bytes);
extern int read_from_path(char* path, offset_t offset, char *buffer, int bytes);

//auto-skips if offset and/or (offset+bytes) is greater than block size.
extern int write_to_block(blockno_t n, offset_t offset, char *buffer, int bytes);
extern int write_to_path(char* path, offset_t offset, char *buffer, int bytes);

extern int block_chain_length(blockno_t start_block);

FILE* get_data_handle(blockno_t blockno, offset_t offset);
//sync the headers to disk. Data should auto-sync(passthrough).
extern void sync();

// Initializes the backing store, using the given path to the backing store file.
// Should be called at the beginning of main.
extern void init_tfs(char* path, bool read_from_backing_storage);

#endif //API_H
