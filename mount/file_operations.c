#include "include/libtfs.h"
#include "mount.h"

int do_read(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    START("do_read");

    /*int i,j;
       LOG( "--> Trying to read %s, %lu, %lu", path, offset, size );
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
       LOG("%s",selectedText);

       if(selectedText == NULL)
       {

            LOG("RETURN -1");
            return -1;
       }
       memcpy( buffer, selectedText + offset, size );
       return strlen( selectedText ) - offset;*/
    char selectedText[100];
    size = 10;
    read_from_path(path, offset, &selectedText, size);
    memcpy( buffer, selectedText + offset, size );
    LOG("read : %s",buffer);
    return strlen( selectedText ) - offset;
}

int do_truncate(char *path, off_t offset, struct fuse_file_info *fi)
{
    START("do_truncate");
    LOG("RETURN 0");
    return 0;
}

int do_write(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    START("do_write");

/*
        int i,j;
        offset = 0;
        LOG("Write Operation");
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
                        LOG("Trying to write to file");

                        LOG("RETURN SIZE %ld",size);
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
    //return write_to_path(path, offset, buffer, size);
    iwrite(getinode(path),buffer);
    return size-offset;
}

int do_create(char *path, mode_t mode, struct fuse_file_info *fi)
{
    START("do_create");
    int i;
    char path2[PATH_LENGTH];
    for (i = 0; i < strlen(path) - 1; i++)
    {
        path2[i] = path[i + 1];
    }
    path2[i] = '\0';
    LOG("Trying to create file %s", path2);
    for (i = 0; i < file_count; i++)
    {
        if (strcmp(strdup(get_path(i)), strdup(path2)) == 0)
        {
            LOG("File already exists.");
            break;
        }
        else
        {
//                        dir[file_count] = 1;
            insert(strdup(path2), 0);
            LOG("Insert Command Executed");

            LOG("RETURN 0");
            return 0;
        }
    }
}
