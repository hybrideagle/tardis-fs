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
#include <assert.h>

#include "libtfs.h"



const char* name[100];
const char* content[100];
int dir[100];

int file_count = 0;

void insert(char* path, int mode)
{
        assert(mode==1 || mode==2)
        //name[file_count] = strdup(path);
        //file_count++;
        if(mode == 0)
            create_file(path);
        else if(mode == 1)
            create_dir(path);
}


const char* get_path(inode_t inode)
{
        //return name[inode];
        return files[inode].path;
}

const char* iread(int inode)
{
        //return content[inode];
        char* buf = (char*)malloc(sizeof(char)*1000);
        read_from_block(files[inode].start_block, 0, buf, 1000);

        return buf;
}

void iwrite(int inode,char* text)
{
        write_to_block(files[inode].start_block, 0, text, strlen(text));
}

int getinode(char* path)
{
        int i, j;
        for(i=0; i<file_count; i++)
        {
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get_path(i));
                char* fpath2 = fpath;
                if(strcmp(path,fpath2)==0 && strlen(path)==strlen(fpath2))
                {
                        return i;
                }
                for(j=0; j<10; j++)
                {
                        fpath[j]='\0';
                }
        }
        return -1;
}

int isd(int inode)
{
        if(dir[inode]==1)
        {
                return 0;
        }
        else if(dir[inode]==2)
        {
                return 1;
        }
        else
        {
                return -1;
        }
}


int rmv(char* path)
{
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
        strcpy(name[i],strdup("@"));
        dir[i] = -1;
        return 0;
}

void display()
{
        int i;
        for(i=0; i<file_count+1; i++)
        {
                printf("\n%s",name[i]);
        }
}

static int do_getattr( const char *path, struct stat *st )
{
        printf( "[getattr] Called\n" );
        printf( "\tAttributes of %s requested\n", path );
        st->st_uid = getuid();
        st->st_gid = getgid();
        st->st_atime = time(NULL);
        st->st_mtime = time(NULL);
        if(strlen(path)==1 && strcmp(strdup(path),strdup("/"))==0)
        {
                st->st_mode = S_IFDIR | 0777;
                st->st_nlink = 2;
                st->st_size = 1024;
                return 0;
        }
        int inode = getinode(strdup(path));
        printf( "\tAttributes of %s[inode : %d] requested\n", path,inode );
        printf("ISD = %d\n",isd(inode));
        if ( isd(inode)==1 )
        {
                printf("DIRECTORY\n");
                st->st_mode = S_IFDIR | 0777;
                st->st_nlink = 2;
                printf("RETURNING 0\n");
                return 0;
        }
        else if ( isd(inode)==0 )
        {
                printf("REGULAR\n");
                st->st_mode = S_IFREG | 0777;
                st->st_nlink = 1;
                st->st_size = 1024;
                printf("RETURNING 0\n");
                return 0;
        }
        else if ( isd(inode)==-1)
        {
                printf("REGULAR\n");
                st->st_mode = S_IFREG | 0777;
                st->st_nlink = 1;
                st->st_size = 1024;
                printf("RETURNING NOENT\n");
                return -ENOENT;
        }
        int i = 0,j = 0;
        for(i=0; i<file_count; i++)
        {
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get_path(i));
                char* fpath2 = fpath;
                if(strcmp(path,fpath2)==0)
                {
                        printf("RETURNING 0\n");
                        return 0;
                }
                for(j=0; j<10; j++)
                {
                        fpath[j]='\0';
                }
        }
        printf("RETURNING NOENT\n");
        return -ENOENT;
}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
{
        int i,j,count,root=0;
        printf( "--> Getting The List of Files of %s\n", path );
        display();
        for(i = 0; i<file_count; i++)
        {
                root = 0, count = 0;
                for(j=0; j<strlen(get_path(i)); j++)
                {
                        printf("Testing : %d\t%c|\n",get_path(i)[j]-'/',get_path(i)[j]);
                        if(get_path(i)[j]=='/')
                        {
                                count++;
                        }
                }
                if(count==0)
                {
                        root = 1;
                }
                printf("\nROOT = %d\n",root);
                count = 0;
                for(j=0; j<strlen(path)-1; j++)
                {
                        if(path[j+1]-get_path(i)[j]!=0)
                        {
                                count++;
                        }
                }
                if(count==0)
                {
                        if(strlen(get_path(i))<strlen(path))
                        {
                                count+=100;
                        }
                }
                int c1=0,c2=0;
                for(j=0; j<strlen(path); j++)
                {
                        if(path[j]-'/'==0)
                        {
                                c1++;
                        }
                }
                printf("%s - %d\n",path,c1);
                for(j=0; j<strlen(get_path(i)); j++)
                {
                        if(get_path(i)[j]-'/'==0)
                        {
                                c2++;
                        }
                }
                if(get_path(i)[strlen(get_path(i))-1]-'/'!=0)
                {
                        c2++;
                }
                if(path[strlen(path)-1]-'/'==0)
                {
                        c1--;
                }
                printf("%s - %d\n",get_path(i), c2);
                if(c2-c1>1)
                {
                        count+=1000;
                }
                if(dir[i] ==-1)
                {
                        printf("Check Path >> %s\n",path);
                        count+=100000;
                }
                printf("\nMismatch = %d\n_____________________\n",count);
                if(((strcmp(path,"/")==0 && root)||count ==0)&&(count<100000))
                {
                        for(j=strlen(get_path(i))-1; j>=0; j--)
                        {
                                if(get_path(i)[j]-'/' == 0)
                                {
                                        break;
                                }
                        }
                        printf("Show > %s , %d\n",get_path(i),j);

                        int s = j;
                        char temp;
                        if(s>0)
                        {
                                char temp[strlen(get_path(i))-j];
                                for(j=0; j<strlen(get_path(i))-j-1; j++)
                                {
                                        printf("Buffering %c\n",strdup(get_path(j))[j+s+1]);
                                        temp[j] = strdup(get_path(i))[j+s+1];
                                }
                                temp[j]='\0';
                                printf("Individual entry : %s\n\n",temp);
                                filler( buffer, strdup(temp), NULL, 0 );
                        }
                        else
                        {
                                printf("%s\n\n",get_path(i));
                                filler( buffer, strdup(get_path(i)), NULL, 0 );
                        }
                }/*
                    if(strlen(path)==1 && root)
                    {
                        filler( buffer, get_path(i), NULL, 0 );
                    }*/
        }
        printf("RETURN 0\n");
        return 0;

}

static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi )
{
        /*int i,j;
        printf( "--> Trying to read %s, %lu, %lu\n", path, offset, size );
        char *selectedText = NULL;

        for(i=0; i<file_count; i++)
        {
                char fpath[10];
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
        printf("\n%s\n",selectedText);

        if(selectedText == NULL)
        {

                printf("RETURN -1\n");
                return -1;
        }
        memcpy( buffer, selectedText + offset, size );
        return strlen( selectedText ) - offset;*/
        return read_from_path(path, offset, buffer, size);
}

const int do_truncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
        printf("RETURN 0\n");
        return 0;
}

static int do_write(const char *path, const char * buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
/*
        int i,j;
        offset = 0;
        printf("\nWrite Operation\n");
        for(i=0; i<file_count; i++)
        {
                char fpath[10];
                fpath[0] = '/';
                strcat(fpath,get_path(i));
                char* fpath2 = fpath;
                if(strcmp(path,fpath2)==0)
                {
                        content[i] = strdup(buffer);
                        size = strlen(buffer);
                        printf("Trying to write to file\n");

                        printf("RETURN SIZE %ld\n",size);
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

static int do_create(const char * path, mode_t mode,struct fuse_file_info *fi)
{
        int i;
        char path2[10];
        for(i=0; i<strlen(path)-1; i++)
        {
                path2[i] = path[i+1];
        }
        path2[i] = '\0';
        printf("\nTrying to create file %s\n",path2);
        for(i=0; i<file_count; i++)
        {

                if(strcmp(strdup(get_path(i)),strdup(path2))==0)
                {
                        printf("\nFile already exists.\n");
                        break;
                }
                else
                {
                        dir[file_count] = 1;
                        insert(strdup(path2));
                        printf("Insert Command Executed\n");

                        printf("RETURN 0\n");
                        return 0;
                }
        }
}

static int do_access(const char *path, int mask)
{
        printf("do_access passthrough\n");
        return 0;
}

static int do_setxattr (const char * path, size_t size)
{
        size = 1024;
        printf("do_setattr passthrough\n");
        return 0;
}
static int do_mkdir(const char * path, mode_t mode)
{
        mode = O_CREAT;
        printf("Mkdir Called\n");
        dir[file_count] = 2;
        int i;
        char path2[10];
        for(i=0; i<strlen(path)-1; i++)
        {
                path2[i] = path[i+1];
        }
        path2[i] = '\0';
        insert(strdup(path2));
        char * cp;
        cp = strdup(path2);
        strcat(cp,"/.");
        dir[file_count] = 2;
        insert(cp);
        cp = strdup(path2);
        strcat(cp,"/..");
        dir[file_count] = 2;
        insert(cp);
        return 0;
}
static int do_unlink(const char* path)
{
        printf("Unlink called.\n");
        rmv(strdup(path));
        printf(">>%d\n",dir[getinode(path)]);
        display();
        return 0;
}

static struct fuse_operations operations = {

        .getattr    = do_getattr,
        .readdir    = do_readdir,
        .read       = do_read,
        .truncate   = do_truncate,
        .write      = do_write,
        .create     = do_create,
        .mkdir      = do_mkdir,
        .mknod      = do_create,
        .access     = do_access,
        .setxattr   = do_setxattr,
        .unlink     = do_unlink,
        .rmdir      = do_unlink,

};

int main( int argc, char *argv[] )
{
        int di;
        for(di=0; di<100; di++)
        {
                dir[di] = -1;
        }

        insert(".");
        insert("..");
        insert("file1");
        insert("file2");
        insert("new");
        iwrite(2,"Heyyy");
        iwrite(3,"Hi");
        dir[0]=1; dir[1]=1; dir[2]=1; dir[3]=1; dir[4]=1;


        return fuse_main( argc, argv, &operations, NULL );
}
