#include "include/libtfs.h"
#include "mount.h"
//char *name[100];
//char *content[100];
//int dir[100];

//int file_count = 0;


static struct fuse_operations operations =
{
    .getattr = do_getattr,
    .readdir = do_readdir,
    .read = do_read,
    .truncate = do_truncate,
    .write = do_write,
    .create = do_create,
    .mkdir = do_mkdir,
    .mknod = do_create,
    .access = do_access,
    .setxattr = do_setxattr,
    .unlink = do_unlink,
    .rmdir = do_unlink,
};

int main(int argc, char *argv[])
{
    START("main");
    assert(argc >= 2);
    LOG1("beginning init");
    init_tfs(argv[1]);
    //remove the second argument
    char *x = calloc(argc - 1, sizeof(char*));
    x[0] = argv[0];
    for (int i = 2; i < argc; i++)
    {
        x[i] = argv[i];
    }
    argc = argc - 2;
    argv = x;


    /*int di;
    for (di = 0; di < 100; di++)
    {
        dir[di] = -1;
    }*/
    LOG1("Setting up initial files");
    insert(".", 0);
    insert("..", 0);
    insert("file1", 0);
    insert("file2", 0);
    insert("new", 0);
    sanity_check();
    LOG1("Done creating up initial files, writing data");
    iwrite(2, "Heyyy");
    iwrite(3, "Hi");
    LOG1("Done setting up initial files");
    //dir[0] = 1; dir[1] = 1; dir[2] = 1; dir[3] = 1; dir[4] = 1;
    sanity_check();

    return fuse_main(argc, argv, &operations, NULL);
}
