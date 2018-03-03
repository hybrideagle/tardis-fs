#include "include/libtfs.h"
#include "mount.h"

void insert(char *path, int mode)
{
    START("insert");
    assertd(path != NULL);
    assertd(mode == 0 || mode == 1);
    //name[file_count] = strdup(path);
    file_count++;
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
    assertd(inode > 0 && inode < file_count);
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
