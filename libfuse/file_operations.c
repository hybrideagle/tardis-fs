#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

/**
* @brief Create a new file in the file table with the given path
* @param [in] path Path of the new file
* @return inode of the new file
*/
inode_t create_file(char *path)
{
   assertd(path != NULL);

    START("create_file");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = true;
            files[inode].is_dir = true;
            //files[inode].path = path
            strcpy(files[inode].path, path);
            files[inode].start_block = get_first_free_block();
            return inode;

        }
    }
    END("create_file");
    return -1;
}


/**
* @brief Remove a file from the file table
* @param [in] path <parameter_description>
* @return true on success, false on failure
* @details <details>
*/
int delete_file(char *path)
{
   assertd(path != NULL);
    START("delete_file");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = false;
            // free(files[inode].path);
            nullify_path_array(files[inode].path);
            delete_block_chain(files[inode].start_block);
            return true;

        }
    }
    END("delete_file");
    return false;
}

/**
* @brief Create a new directory in the file table
* @param [in] path full path to the new directory
* @return inode number of the new directory
*/
inode_t create_dir(char *path)
{
   assertd(path != NULL);
    START("create_dir");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        if (!files[inode].used)
        {
            files[inode].used = true;
            files[inode].is_dir = true;
            //files[inode].path = path
            strcpy(files[inode].path, path);
            files[inode].start_block = get_first_free_block();
            return inode;

        }
    }
    END("create_dir");
    return -1;
}

/**
* @brief Delete a directory from the file table
* @param [in] path
* @return success?
* @details <details>
*/
int delete_dir(char *path)
{
   assertd(path != NULL);
    START("delete_dir");
    // TODO check for subfiles and shit
    return delete_file(path);

    END("delete_dir");
}

/**
   read `bytes` bytes from the file into `buffer`.
   `block` points to the beginning of the file
   returns 1 on failure
   No bound checking occurs, segfaults are your problem
 */
