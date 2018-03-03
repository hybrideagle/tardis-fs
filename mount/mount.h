#ifndef MOUNT_H
#define MOUNT_H

int do_getattr(char *path, struct stat *st);
int do_readdir(char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
int do_read(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
int do_truncate(char *path, off_t offset, struct fuse_file_info *fi);
int do_write(char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi);
int do_create(char *path, mode_t mode, struct fuse_file_info *fi);
int do_access(char *path, int mask);
int do_setxattr(char *path, size_t size);
int do_mkdir(char *path, mode_t mode);
int do_unlink(char *path);
int main(int argc, char *argv[]);
void insert(char *path, int mode);
void iwrite(int inode, char *text);
int getinode(char *path);
int isd(inode_t inode);
int rmv(char *path);
void display();
char* get_path(inode_t inode);
void sanity_check();
#endif
