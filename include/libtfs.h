/**
* @file libtfs.h
* @author Sumeet Padavala
* @date 3 Mar 2018
* @copyright 2018 Sumeet Padavala
* @brief This header serves as the interface of the frontend code to the routines and structures that implement the tfs system.
*
*/

#ifndef LIBTFS_H
#define LIBTFS_H

#define FUSE_USE_VERSION 30

#include "defs.h"
#include "api.h"

/* Structure of the backing storage:
    The backing storage should contain binary data in the form:
    1. Header:[sizeof(struct file)*NUM_FILES + sizeof(struct blocks)*NUM_BLOCKS bytes]
        1. An array of `struct file`, length NUM_FILES   -> files
        2. An array of `struct block`, length NUM_BLOCKS -> blocks
    3. The actual block data in an array of `BLOCKSIZE` bytes, length NUM_BLOCKS
        [NUM_BLOCKS*BLOCKSIZE bytes]
    Sync should write all of the header to disk, data should be written immediately.
 */



#endif //LIBTFS_H
