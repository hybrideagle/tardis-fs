#define FUSE_USE_VERSION 30

#include <unistd.h>
#include <fuse.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "libtfs.h"


const char* name[100];
const char* content[100];
int ilen = 0;

void insert(char* path)
{
        name[ilen] = strdup(path);
        ilen++;
}

const char* get(int inode)
{
        return name[inode];
}

const char* iread(int inode)
{
        return content[inode];
}

void iwrite(int inode,char* text)
{
        content[inode] = strdup(text);
}

static int do_getattr( const char *path, struct stat *st )
{
        printf( "[getattr] Called\n" );
        printf( "\tAttributes of %s requested\n", path );
        st->st_uid = getuid();
        st->st_gid = getgid();
        st->st_atime = time(NULL);
        st->st_mtime = time(NULL);

        if ( strcmp( path, "/" ) == 0 )
        {
                st->st_mode = S_IFDIR | 0777;
                st->st_nlink = 2;
                return 0;
        }
        else
        {
                st->st_mode = S_IFREG | 0777;
                st->st_nlink = 1;
                st->st_size = 1024;
        }
        int i = 0,j = 0;
        for(i=0; i<ilen; i++)
        {
                printf("%s,%s\n",strdup(path),get(i));
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get(i));
                char* fpath2 = fpath;
                if(strcmp(path,fpath2)==0)
                {
                        printf("%s Found\n",strdup(path));
                        return 0;
                }
                for(j=0; j<10; j++)
                {
                        fpath[j]='\0';
                }
        }
        printf("%s Not Found\n",strdup(path));
        return -ENOENT;
}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
        int i;
        printf( "--> Getting The List of Files of %s\n", path );

        for(i = 0; i<ilen; i++)
        {
                filler( buffer, get(i), NULL, 0 );
        }
        return 0;

}

static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
        int i,j;
        printf( "--> Trying to read %s, %lu, %lu\n", path, offset, size );
        char *selectedText = NULL;

        for(i=0; i<ilen; i++)
        {
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get(i));
                char* fpath2 = fpath;
                //printf("\n%s\n",fpath2);
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
        printf("\n%s\n",selectedText);

        if(selectedText == NULL)
                return -1;

        memcpy( buffer, selectedText + offset, size );
        return strlen( selectedText ) - offset;
}

const int do_truncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
        return 0;
}

static int do_write(const char *path, const char * buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{

        int i,j;
        offset = 0;
        printf("\nWrite Operation\n");
        for(i=0; i<ilen; i++)
        {
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get(i));
                char* fpath2 = fpath;
                //printf("\n%s\n",fpath2);
                if(strcmp(path,fpath2)==0)
                {
                        content[i] = strdup(buffer);
                        size = strlen(buffer);
                        printf("Trying to write to file\n");
                        return size;
                        break;
                }
                for(j=0; j<10; j++)
                {
                        fpath[j]='\0';
                }
        }
        return size;

}

static int do_create(const char * path, mode_t mode,struct fuse_file_info *fi)
{
        int i;
        printf("\nTrying to create file\n");
        //for(i=0;i<ilen;i++)
        //{
        /*
           if(strcmp(strdup(get(i)),strdup(path)))
           {
            printf("\nFile already exists.\n");
            break;
           }
           else
           {
         */
        mode = 0777;
        insert(strdup(path));
        return 0;
        //}
        //}
}

static int do_access(const char *path, int mask)
{
        return 0;
}

int do_setxattr(const char * path, size_t size)
{
        size = 1024;
        return 0;
}

static struct fuse_operations operations = {

        .getattr    = do_getattr,
        .readdir    = do_readdir,
        .read       = do_read,
        .truncate   = do_truncate,
        .write      = do_write,
        .create     = do_create,
        //.mkdir		= do_mkdir,
        //.mknod		= do_create,
        .access   = do_access,
        .setxattr = do_setxattr,

};

int main( int argc, char *argv[] )
{
        init_tfs(argv[1]);
        insert(".");
        insert("..");
        insert("file1");
        insert("file2");
        insert("new");
        iwrite(4,"Hello");
        iwrite(2,"Heyyy");
        iwrite(3,"Hi");


        return fuse_main( argc, argv, &operations, NULL );
}
