#include "../include/libtfs.h"
#include "mount.h"
#include <stddef.h>
//char *name[100];
//char *content[100];
//int dir[100];

//int file_count = 0;


static struct fuse_operations operations =
{
    //attribute functions
    .getattr = do_getattr,
    .setxattr = do_setxattr,
    .access = do_access,
    .utimens = do_utimens,
    //file functions
    .read = do_read,
    .mknod = do_create,
    .truncate = do_truncate,
    .write = do_write,
    .create = do_create,
    //directory functions
    .readdir = do_readdir,
    .mkdir = do_mkdir,
    .unlink = do_unlink,
    .rmdir = do_unlink,
};



enum keys{ KEY_HELP=10, KEY_PATH=50,
};

//#define TFS_OPT(t, p, v) { t, 0, v }

struct temp{char* path};


static struct fuse_opt tfs_opts[] =
{

    FUSE_OPT_KEY("-h", KEY_HELP),
    FUSE_OPT_KEY("--help", KEY_HELP),
    FUSE_OPT_END
};

static int tfs_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs)
{
    START("tfs_opt_proc, %d", key);
    switch (key)
    {
    case KEY_HELP:
        fprintf(stderr,
                "usage: %s mountpoint [options]\n"
                "\n"
                "general options:\n"
                "    -o opt,[opt...]  mount options\n"
                "    -h   --help      print help\n"
                "    -V   --version   print version\n"
                "\n"
                "tfs options:\n"
                "    -o path=string\n"
                , outargs->argv[0]);

        exit(1);
        return 1;
    case KEY_PATH:
        LOG1("Read path:(%s)", data);
        backing_storage_path = (char*)data;
        return 0;
    }
}



int main(int argc, char *argv[])
{
    init_tfs(NULL,true);
    dump_data();
    init_logging();
    START("main");
    //numassert(argc >= 3, argc);

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    struct temp conf;

    memset(&conf, 0, sizeof(conf));
    fuse_opt_parse(&args, &conf, tfs_opts, tfs_opt_proc);


    LOG1("Setting up initial files");
    insert(".", 0);
    insert("..", 0);
    insert("file1", 0);
    insert("file2", 0);
    insert("new", 0);
    sanity_check();
    dump_data();
    LOG1("Done creating up initial files, writing data");
    iwrite(2, "Heyyy");
    iwrite(3, "Hi");
    LOG1("Done setting up initial files");

    //sanity_check();
    //dump_data();

    LOG1("Deferring to fuse_main...");
    return fuse_main(args.argc, args.argv, &operations, NULL);
}
