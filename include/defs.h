#ifndef DEFS_H
#define DEFS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fuse.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define BLOCKSIZE 10 //bytes
#define NUM_FILES 4 //number of files(inodes)
#define NUM_BLOCKS 10 //number of blocks
#define PATH_LENGTH 10 //number of characters in each path


// Global, holds the full path to the backing storage file
char* backing_storage_path;

FILE* backing_storage;
// Type alias for referencing offset within a block
typedef int offset_t;
// Type alias for referncing block numbers
typedef int blockno_t;
// type alias for referencing file record offset
typedef int inode_t;


//typedef struct stats {
//        int st_ino; /* inode number */
//        int st_mode; /* protection */
//        unsigned int st_uid; /* user ID of owner */
//        unsigned int st_gid; /* group ID of owner */
//    long st_size; /* total size, in bytes */
//    long st_atime; /* time of last access */
//    long st_mtime; /* time of last modification */
//    long st_ctime; /* time of last status change */
//}stats;

typedef struct file {
        char path[PATH_LENGTH];
        blockno_t start_block;
        bool is_dir;
        bool used;
//        stats stat;
}file;

// Current max inode
inode_t file_count;

typedef struct block {
        blockno_t next; /* block number of the next block, -1 if N/A */
        bool allocated; /* is this block in use? */
}block;


file files[NUM_FILES];
block blocks[NUM_BLOCKS];

offset_t files_origin;
offset_t blocks_origin;//sizeof(struct file)*NUM_FILES;
offset_t data_origin;//sizeof(struct file)*NUM_FILES + sizeof(struct blocks)*NUM_BLOCKS;

void init_logging();

FILE* log_file;

// Logging macros and constants
#define START(args...) printf("(%s)[START]", __FILE__);printf(args);fflush(stdout);printf("\n");
#define END(args...)   printf("[END]");printf(args);fflush(stdout);printf("\n");
#define LOG(args...)   printf("[LOG]");printf(args);fflush(stdout);printf("\n");
#define LOG1(args...)  printf("\t[LOG1]");printf(args);fflush(stdout);printf("\n");

//Delimited assert
#define assertd(x) if(!(x))printf("\n\n###########\n");assert(x);
//assertation that prints out a numeric value on crash
#define numassert(x, num) if(!(x)) {printf("\n\n###########\n"); printf("%s:%d:assertion failed(%s:[%s=%d])", __FILE__, __LINE__, #x, #num, num); abort();}


#endif //DEFS_H
