#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "libtfs.h"

//TODO make this function respect half-filled blocks
int block_chain_length(blockno_t start_block)
{
    START("block_chain_length");
    int size = 0;
    do
    {
        size += BLOCKSIZE;
        start_block = blocks[start_block].next;
    } while (start_block != -1);
    END("block_chain_length");
    return size;
}

blockno_t get_first_block_from_inode(inode_t inode)
{
    assert(inode >= 0 && inode < NUM_FILES);
    START("get_first_block_from_inode");
    END("get_first_block_from_inode");
    return files[inode].start_block;

}

blockno_t get_next_block(blockno_t blockno)
{
    assert(blockno >= 0 && blockno < NUM_BLOCKS);
    START("get_next_block");
    END("get_next_block");
    return blocks[blockno].next;

}

blockno_t get_or_create_next_block(blockno_t blockno)
{
    assert(blockno >= 0 && blockno < NUM_BLOCKS);
    START("get_or_create_next_block");
    // If next isn't allocated, allocate it
    if (blocks[blockno].next == -1)
    {
        //TODO finish this
        for (blockno_t next = 0; next < NUM_BLOCKS; next++)
        {
            if (blocks[next].allocated == false)
            {
                blocks[blockno].next = next;
                assert(blocks[next].next == -1);
                blocks[next].allocated = true;
            }
        }
    }

    END("get_or_create_next_block");
    return blocks[blockno].next;
}

blockno_t get_first_free_block()
{
    START("get_first_free_block");
    for (blockno_t i = 0; i < NUM_BLOCKS; i++)
    {
        if (blocks[i].allocated == false)
        {
            return i;

        }
    }
    END("get_first_free_block");
    return -1;
}

bool delete_block_chain(blockno_t start_block)
{
    START("delete_block_chain");
    assert(start_block > 0 && start_block < NUM_BLOCKS);
    if (start_block == -1)
    {
        return false;
    }
    blockno_t curr_block = start_block;

    do
    {
        blocks[curr_block].next = -1;
        blocks[curr_block].allocated = false;
        curr_block = blocks[curr_block].next;
    } while (curr_block != -1);

    END("delete_block_chain");
    return true;
}
