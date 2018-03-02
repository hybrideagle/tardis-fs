#include "libtfs.h"

//char *name[100];
//char *content[100];
//int dir[100];

//int file_count = 0;

void insert(char *path, int mode)
{
    START("insert");
    assert(path != NULL);
    assert(mode == 0 || mode == 1);
    //name[file_count] = strdup(path);
    //file_count++;
    if (mode == 0)
    {
        create_file(path);
    }
    else if (mode == 1)
    {
        create_dir(path);
    }
}

char *get_path(inode_t inode)
{
    START("get_path");
    assert(inode > 0 && inode < file_count);
    //return name[inode];
    return files[inode].path;
}

char *iread(inode_t inode)
{
    START("iread");
    //return content[inode];
    char *buf = (char *)malloc(sizeof(char) * 1000);
    read_from_block(files[inode].start_block, 0, buf, 1000);

    return buf;
}

void iwrite(int inode, char *text)
{
    START("iwrite");
    write_to_block(files[inode].start_block, 0, text, strlen(text));
}

//TODO verify this works
int getinode(char *path)
{
    START("getinode");
    int i, j;
    for (i = 0; i < file_count; i++)
    {
        char fpath[PATH_LENGTH];
        fpath[0] = '/';
        strcat(fpath, get_path(i));
        char *fpath2 = fpath;
        if (strcmp(path, fpath2) == 0 && strlen(path) == strlen(fpath2))
        {
            return i;
        }
        // reset for the next run
        for (j = 0; j < 10; j++)
        {
            fpath[j] = '\0';
        }
    }
    return -1;
}

int isd(int inode)
{
    START("isd");
    if (files[inode].used == false)
    {
        return -1;
    }
    else if (files[inode].is_dir == true)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int rmv(char *path)
{
    START("rmv");
    int i = getinode(path);
    int j;

    /*
       for(j=0; j<strlen(path[j]); j++)
       {
            name[i][j]='\0';
       }
       for(j=0; j<strlen(content[j]; j++))
       {
            content[i][j]='\0';
       }
       dir[i] = -1;
     */
    strcpy(files[i].path, strdup("@"));
    //dir[i] = -1;
    return 0;
}

void display()
{
    START("display");
    int i;
    for (i = 0; i < file_count + 1; i++)
    {
        LOG("\n%s", files[i].path);
    }
}

static int do_getattr(char *path, struct stat *st)
{
    assert(path != NULL);
    START("do_getattr");
    LOG("[getattr] Called\n");
    LOG("\tAttributes of %s requested\n", path);
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);
    if (strlen(path) == 1 && strcmp(strdup(path), strdup("/")) == 0)
    {
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 2;
        st->st_size = 1024;
        return 0;
    }
    int inode = getinode(strdup(path));
    LOG("\tAttributes of %s[inode : %d] requested\n", path, inode);
    LOG("ISD = %d\n", isd(inode));
    if (isd(inode) == 1)
    {
        LOG("DIRECTORY\n");
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 2;
        LOG("RETURNING 0\n");
        return 0;
    }
    else if (isd(inode) == 0)
    {
        LOG("REGULAR\n");
        st->st_mode = S_IFREG | 0777;
        st->st_nlink = 1;
        st->st_size = 1024;
        LOG("RETURNING 0\n");
        return 0;
    }
    else if (isd(inode) == -1)
    {
        LOG("REGULAR\n");
        st->st_mode = S_IFREG | 0777;
        st->st_nlink = 1;
        st->st_size = 1024;
        LOG("RETURNING NOENT\n");
        return -ENOENT;
    }
    int i = 0, j = 0;
    for (i = 0; i < file_count; i++)
    {
        char fpath[PATH_LENGTH];
        fpath[0] = '/';
        strcat(fpath, get_path(i));
        char *fpath2 = fpath;
        if (strcmp(path, fpath2) == 0)
        {
            LOG("RETURNING 0\n");
            return 0;
        }
        for (j = 0; j < 10; j++)
        {
            fpath[j] = '\0';
        }
    }
    LOG("RETURNING NOENT\n");
    return -ENOENT;
}

static int do_readdir(char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    START("do_readdir");
    int i, j, count, root = 0;
    LOG("--> Getting The List of Files of %s\n", path);
    display();
    for (i = 0; i < file_count; i++)
    {
        root = 0, count = 0;
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            LOG("Testing : %d\t%c|\n", get_path(i)[j] - '/', get_path(i)[j]);
            if (get_path(i)[j] == '/')
            {
                count++;
            }
        }
        if (count == 0)
        {
            root = 1;
        }
        LOG("\nROOT = %d\n", root);
        count = 0;
        for (j = 0; j < strlen(path) - 1; j++)
        {
            if (path[j + 1] - get_path(i)[j] != 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            if (strlen(get_path(i)) < strlen(path))
            {
                count += 100;
            }
        }
        int c1 = 0, c2 = 0;
        for (j = 0; j < strlen(path); j++)
        {
            if (path[j] - '/' == 0)
            {
                c1++;
            }
        }
        LOG("%s - %d\n", path, c1);
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            if (get_path(i)[j] - '/' == 0)
            {
                c2++;
            }
        }
        if (get_path(i)[strlen(get_path(i)) - 1] - '/' != 0)
        {
            c2++;
        }
        if (path[strlen(path) - 1] - '/' == 0)
        {
            c1--;
        }
        LOG("%s - %d\n", get_path(i), c2);
        if (c2 - c1 > 1)
        {
            count += 1000;
        }
        // TODO figure out how to fix this monster code
        /*
if (dir[i] == -1)
        {
            LOG("Check Path >> %s\n", path);
            count += 100000;
        }*/


        LOG("\nMismatch = %d\n_____________________\n", count);
        if (((strcmp(path, "/") == 0 && root) || count == 0) && (count < 100000))
        {
            for (j = strlen(get_path(i)) - 1; j >= 0; j--)
            {
                if (get_path(i)[j] - '/' == 0)
                {
                    break;
                }
            }
            LOG("Show > %s , %d\n", get_path(i), j);

            int s = j;
            char temp;
            if (s > 0)
            {
                char temp[strlen(get_path(i)) - j];
                for (j = 0; j < strlen(get_path(i)) - j - 1; j++)
                {
                    LOG("Buffering %c\n", strdup(get_path(j))[j + s + 1]);
                    temp[j] = strdup(get_path(i))[j + s + 1];
                }
                temp[j] = '\0';
                LOG("Individual entry : %s\n\n", temp);
                filler(buffer, strdup(temp), NULL, 0);
            }
            else
            {
                LOG("%s\n\n", get_path(i));
                filler(buffer, strdup(get_path(i)), NULL, 0);
            }
        }        /*
                    if(strlen(path)==1 && root)
                    {
                        filler( buffer, get_path(i), NULL, 0 );
                    }*/
    }
    LOG("RETURN 0\n");
    return 0;
}

static int do_read(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    START("do_read");

    /*int i,j;
       LOG( "--> Trying to read %s, %lu, %lu\n", path, offset, size );
       char *selectedText = NULL;

       for(i=0; i<file_count; i++)
       {
            char fpath[PATH_LENGTH];
            fpath[0] = '/';
            strcat(fpath,get_path(i));
            char* fpath2 = fpath;
            if(strcmp(path,fpath2)==0)
            {
                    selectedText = strdup(iread(i));
                    break;
            }
            for(j=0; j<10; j++)
            {
                    fpath[j]='\0';
            }
       }
       LOG("\n%s\n",selectedText);

       if(selectedText == NULL)
       {

            LOG("RETURN -1\n");
            return -1;
       }
       memcpy( buffer, selectedText + offset, size );
       return strlen( selectedText ) - offset;*/
    return read_from_path(path, offset, buffer, size);
}

int do_truncate(char *path, off_t offset, struct fuse_file_info *fi)
{
    START("do_truncate");
    LOG("RETURN 0\n");
    return 0;
}

static int do_write(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    START("do_write");

/*
        int i,j;
        offset = 0;
        LOG("\nWrite Operation\n");
        for(i=0; i<file_count; i++)
        {
                char fpath[PATH_LENGTH];
                fpath[0] = '/';
                strcat(fpath,get_path(i));
                char* fpath2 = fpath;
                if(strcmp(path,fpath2)==0)
                {
                        content[i] = strdup(buffer);
                        size = strlen(buffer);
                        LOG("Trying to write to file\n");

                        LOG("RETURN SIZE %ld\n",size);
                        return size;
                        break;
                }
                for(j=0; j<10; j++)
                {
                        fpath[j]='\0';
                }
        }
        return size;
 */
    return write_to_path(path, offset, buffer, size);
}

static int do_create(char *path, mode_t mode, struct fuse_file_info *fi)
{
    START("do_create");
    int i;
    char path2[PATH_LENGTH];
    for (i = 0; i < strlen(path) - 1; i++)
    {
        path2[i] = path[i + 1];
    }
    path2[i] = '\0';
    LOG("\nTrying to create file %s\n", path2);
    for (i = 0; i < file_count; i++)
    {
        if (strcmp(strdup(get_path(i)), strdup(path2)) == 0)
        {
            LOG("\nFile already exists.\n");
            break;
        }
        else
        {
//                        dir[file_count] = 1;
            insert(strdup(path2), 0);
            LOG("Insert Command Executed\n");

            LOG("RETURN 0\n");
            return 0;
        }
    }
}

static int do_access(char *path, int mask)
{
    START("do_access");
    LOG("do_access passthrough\n");
    return 0;
}

static int do_setxattr(char *path, size_t size)
{
    START("");
    size = 1024;
    LOG("do_setattr passthrough\n");
    return 0;
}

static int do_mkdir(char *path, mode_t mode)
{
    START("do_mkdir");
    mode = O_CREAT;
    LOG("Mkdir Called\n");
    files[file_count].is_dir = true;
    int i;
    char path2[PATH_LENGTH];
    for (i = 0; i < strlen(path) - 1; i++)
    {
        path2[i] = path[i + 1];
    }
    path2[i] = '\0';
    insert(strdup(path2), 1);
    char *cp;
    cp = strdup(path2);
    strcat(cp, "/.");
    //dir[file_count] = 2;
    insert(cp, 0);
    cp = strdup(path2);
    strcat(cp, "/..");
    files[file_count].is_dir = true;
    insert(cp, 0);
    return 0;
}

static int do_unlink(char *path)
{
    START("do_unlink");
    LOG("Unlink called.\n");
    rmv(strdup(path));
    //LOG(">>%d\n", dir[getinode(path)]);
    LOG(">>%d\n", files[getinode(path)].is_dir);
    display();
    return 0;
}

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
    LOG1("Done creating up initial files, writing data");
    iwrite(2, "Heyyy");
    iwrite(3, "Hi");
    LOG1("Done setting up initial files");
    //dir[0] = 1; dir[1] = 1; dir[2] = 1; dir[3] = 1; dir[4] = 1;


    return fuse_main(argc, argv, &operations, NULL);
}
