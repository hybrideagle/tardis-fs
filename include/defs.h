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

#define BLOCKSIZE 4 //bytes
#define NUM_FILES 10 //number of files(inodes)
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

/*!
   \brief A single entry in the file table, stores metadata of a single file
*/
typedef struct file {
        char path[PATH_LENGTH]; /*!< A fixed-length array storing the path(null-terminated) */
        blockno_t start_block; /*!< First data block of the file */
        int is_dir; /*!< Is this file a directory? */
        int used; /*!< Is this file table entry in use? */
//        stats stat;
}file;

// Current max inode
inode_t file_count; /*!< Total count of allocated inodes */

/*!
   \brief Stores metadata of a single block
*/
typedef struct block{
        blockno_t next; /*<! block number of the next block, -1 if N/A */
        int allocated; /*<! is this block in use? */
}block;


file files[NUM_FILES];/*!< Array of struct file, stores the file table */
block blocks[NUM_BLOCKS];/*!< Array of block structures, stores block metadata */

offset_t files_origin;
offset_t blocks_origin;//sizeof(struct file)*NUM_FILES;
offset_t data_origin;//sizeof(struct file)*NUM_FILES + sizeof(struct blocks)*NUM_BLOCKS;


#endif //DEFS_H
