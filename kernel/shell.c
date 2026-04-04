#include "../include/pmm.h"
#include "../include/fs.h"

#define MAX_CMD 80

static char cmd_buf[MAX_CMD];
static int  cmd_len = 0;

static int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] - b[i];
}

static int strncmp(const char* a, const char* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (!a[i]) return 0;
    }
    return 0;
}

static int strlen(const char* s) {
    int i = 0;
    while (s[i]) i++;
    return i;
}

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);
extern void clear();

static void shell_clear() {
    clear();
    print("MyKernel v0.1", 0x0A);
    newline();
    print("Keyboard: OK", 0x0A);
    newline();
    print("Type 'help' for commands", 0x08);
    newline();
    print("----------------------------", 0x08);
    newline();
}

static void run_command() {
    cmd_buf[cmd_len] = 0;

    if (cmd_len == 0) {

    } else if (strcmp(cmd_buf, "help") == 0) {
        newline();
        print("Commands:", 0x0B);
        newline();
        print("  help              — show this list", 0x0F);
        newline();
        print("  hello             — say hello", 0x0F);
        newline();
        print("  clear             — clear screen", 0x0F);
        newline();
        print("  about             — about MyKernel", 0x0F);
        newline();
        print("  maths             — calculator", 0x0F);
        newline();
        print("  mem               — memory stats", 0x0F);
        newline();
        print("  ls                — list files", 0x0F);
        newline();
        print("  write [file.txt]  — create file", 0x0F);
        newline();
        print("  read  [file.txt]  — read file", 0x0F);
        newline();
        print("  del   [file.txt]  — delete file", 0x0F);
        newline();

    } else if (strcmp(cmd_buf, "hello") == 0) {
        newline();
        print("Hello! Welcome to MyKernel!", 0x0A);
        newline();

    } else if (strcmp(cmd_buf, "clear") == 0) {
        shell_clear();

    } else if (strcmp(cmd_buf, "about") == 0) {
        newline();
        print("MyKernel v0.1", 0x0B);
        newline();
        print("Built from scratch in C + Assembly", 0x0F);
        newline();
        print("Running on QEMU x86 virtual machine", 0x0F);
        newline();
        print("Developer: Minha", 0x0A);
        newline();

    } else if (strcmp(cmd_buf, "maths") == 0) {
        newline();
        print("2 + 2 = 4", 0x0F);   newline();
        print("10 x 10 = 100", 0x0F); newline();
        print("2026 / 2 = 1013", 0x0F); newline();

    } else if (strcmp(cmd_buf, "mem") == 0) {
        newline();
        pmm_print_stats();

    } else if (strcmp(cmd_buf, "ls") == 0) {
        newline();
        fs_list();

    } else if (strncmp(cmd_buf, "write ", 6) == 0) {
        // write filename.txt
        const char* filename = cmd_buf + 6;
        if (strlen(filename) == 0) {
            newline();
            print("Usage: write filename.txt", 0x04);
            newline();
        } else {
            newline();
            fs_create(filename, "empty file");
        }

    } else if (strncmp(cmd_buf, "read ", 5) == 0) {
        // read filename.txt
        const char* filename = cmd_buf + 5;
        if (strlen(filename) == 0) {
            newline();
            print("Usage: read filename.txt", 0x04);
            newline();
        } else {
            newline();
            fs_read(filename);
        }

    } else if (strncmp(cmd_buf, "del ", 4) == 0) {
        // del filename.txt
        const char* filename = cmd_buf + 4;
        if (strlen(filename) == 0) {
            newline();
            print("Usage: del filename.txt", 0x04);
            newline();
        } else {
            newline();
            fs_delete(filename);
        }

    } else {
        newline();
        print("Unknown command: ", 0x04);
        print(cmd_buf, 0x04);
        newline();
        print("Type 'help' for commands", 0x08);
        newline();
    }

    cmd_len = 0;
    cmd_buf[0] = 0;
}

void shell_putchar(char c) {
    if (c == '\n') {
        run_command();
        newline();
        print("> ", 0x0F);
    } else if (c == '\b') {
        if (cmd_len > 0) {
            cmd_len--;
            cmd_buf[cmd_len] = 0;
        }
    } else {
        if (cmd_len < MAX_CMD - 1) {
            cmd_buf[cmd_len] = c;
            cmd_len++;
        }
    }
}

void shell_init() {
    cmd_len = 0;
    cmd_buf[0] = 0;
}