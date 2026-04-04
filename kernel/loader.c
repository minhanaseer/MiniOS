#include "../include/loader.h"

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);

static int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] - b[i];
}

static void print_num(int n) {
    if (n == 0) { putchar('0', 0x0F); return; }
    char buf[12]; int i = 0;
    while (n > 0) { buf[i++] = '0'+(n%10); n/=10; }
    for (int j=i-1;j>=0;j--) putchar(buf[j], 0x0F);
}

void loader_init() {}

int loader_run(const char* name) {

    if (strcmp(name, "hello") == 0) {
        newline();
        print("Hello from MyKernel program!", 0x0A);
        newline();
        print("This is a program running on your OS!", 0x0F);
        newline();
        return 0;
    }

    if (strcmp(name, "count") == 0) {
        newline();
        print("Counting 1 to 10:", 0x0B);
        newline();
        for (int i = 1; i <= 10; i++) {
            print("  ", 0x0F);
            print_num(i);
            newline();
        }
        return 0;
    }

    if (strcmp(name, "info") == 0) {
        newline();
        print("System Info:", 0x0B);
        newline();
        print("  OS      : MyKernel v0.1", 0x0F); newline();
        print("  Arch    : x86 32-bit", 0x0F);    newline();
        print("  CPU     : i386 (QEMU)", 0x0F);   newline();
        print("  RAM     : 32 MB", 0x0F);          newline();
        print("  Storage : RAM only", 0x0F);       newline();
        print("  Dev     : Minha", 0x0A);          newline();
        return 0;
    }

    if (strcmp(name, "banner") == 0) {
        newline();
        print(" __  __       _  __                    _ ", 0x0B); newline();
        print("|  \\/  |_   _| |/ /___ _ __ _ __   ___| |", 0x0B); newline();
        print("| |\\/| | | | | ' // _ \\ '__| '_ \\ / _ \\ |", 0x0B); newline();
        print("| |  | | |_| | . \\  __/ |  | | | |  __/ |", 0x0B); newline();
        print("|_|  |_|\\__, |_|\\_\\___|_|  |_| |_|\\___|_|", 0x0B); newline();
        print("        |___/", 0x0B); newline();
        newline();
        return 0;
    }

    print("Program not found: ", 0x04);
    print(name, 0x04);
    newline();
    print("Available: hello, count, info, banner", 0x08);
    newline();
    return -1;
}