#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define BLOCKSIZE 1024 //bytes
#define NUM_FILES 1000 //number of files
#define NUM_BLOCKS 1024 //number of blocks

// Global, holds the full path to the backing storage file
char* backing_storage_path;

// Global, holds a file pointer to the backing storage
FILE* backing_storage;

// Type alias for referencing offset within a block
typedef int offset_t;
// Type alias for referncing block numbers
typedef int blockno_t;
// type alias for referencing file record offset
typedef int inode_t;

struct stats {
        int st_ino;     /* inode number */
        int st_mode;    /* protection */
        unsigned int st_uid;   /* user ID of owner */
        unsigned int st_gid;   /* group ID of owner */
        long st_size; /* total size, in bytes */
        long st_atime; /* time of last access */
        long st_mtime; /* time of last modification */
        long st_ctime; /* time of last status change */
};
struct file {
        char* path;
        blockno_t start_block;
        bool used;
};
inode_t file_count;
struct block {
        blockno_t next; /* block number of the next block, -1 if N/A */
        bool allocated; /* is this block in use? */
};



struct file files[NUM_FILES];
struct block blocks[NUM_BLOCKS];

inode_t create_file(char* path);
int delete_file(char* path);

//for each block(as the index of the array), stores the next block's number
blockno_t get_first_block_from_path(char* path);
blockno_t get_first_block_from_inode(inode_t inode);
blockno_t get_next_block();
blockno_t get_first_free_block();
// void delete_block(blockno_t); //Don't think we'll need this
bool delete_block_chain(blockno_t);
// basic read and write function with 2 ways of accessing the file.
// Returns 0 on success, -1 on failure
// It's your problem if it segfaults because bytes > len(buffer)
int read_from_block(blockno_t n, char* buffer, int bytes, int offset);
int read_from_path(char* path, char* buffer, int bytes, int offset);

//auto-skips if offset and/or (offset+bytes) is greater than block size.
int write_to_block(blockno_t n, char* buffer, int bytes, int offset);
int write_to_path(char* path, char* buffer, int bytes, int offset);

//sync everything to disk. Not sure if anyone needs this, but it's here anyway
void sync();

// Initializes the backing store, using the given path to the backing store file.
// Should be called at the beginning of main.
void init_tfs(char* path);
