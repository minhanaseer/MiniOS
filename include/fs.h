#ifndef FS_H
#define FS_H

#define MAX_FILES     16
#define MAX_FILENAME  32
#define MAX_FILESIZE  256

typedef struct {
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
    int  size;
    int  used;
} File;

void fs_init();
int  fs_create(const char* name, const char* data);
int  fs_read(const char* name);
void fs_list();
int  fs_delete(const char* name);

#endif