#include "../include/fs.h"

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);

static File files[MAX_FILES];
static int  file_count = 0;

static int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] - b[i];
}

static void strcpy(char* dst, const char* src) {
    int i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static int strlen(const char* s) {
    int i = 0;
    while (s[i]) i++;
    return i;
}

void fs_init() {
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].used = 0;
        files[i].size = 0;
        files[i].name[0] = 0;
        files[i].data[0] = 0;
    }
    file_count = 0;
}

int fs_create(const char* name, const char* data) {
    if (file_count >= MAX_FILES) {
        print("Error: max files reached!", 0x04);
        newline();
        return -1;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            print("Error: file already exists!", 0x04);
            newline();
            return -1;
        }
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            strcpy(files[i].name, name);
            strcpy(files[i].data, data);
            files[i].size = strlen(data);
            files[i].used = 1;
            file_count++;
            print("File created: ", 0x0A);
            print(name, 0x0A);
            newline();
            return 0;
        }
    }
    return -1;
}

int fs_read(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            print(files[i].data, 0x0F);
            newline();
            return 0;
        }
    }
    print("Error: file not found!", 0x04);
    newline();
    return -1;
}

void fs_list() {
    if (file_count == 0) {
        print("No files found.", 0x08);
        newline();
        return;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            print("  ", 0x0F);
            print(files[i].name, 0x0B);
            print("  (", 0x08);
            int s = files[i].size;
            if (s == 0) {
                putchar('0', 0x08);
            } else {
                char buf[8]; int j = 0;
                while (s > 0) { buf[j++] = '0'+(s%10); s/=10; }
                for (int k=j-1;k>=0;k--) putchar(buf[k], 0x08);
            }
            print(" bytes)", 0x08);
            newline();
        }
    }
}

int fs_delete(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            files[i].used = 0;
            files[i].size = 0;
            files[i].name[0] = 0;
            files[i].data[0] = 0;
            file_count--;
            print("Deleted: ", 0x0A);
            print(name, 0x0A);
            newline();
            return 0;
        }
    }
    print("Error: file not found!", 0x04);
    newline();
    return -1;
}